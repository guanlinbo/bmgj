#include "choosealecture.h"
#include "ui_choosealecture.h"
#include<QMessageBox>
chooseALecture::chooseALecture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chooseALecture)
{
    ui->setupUi(this);
    m_net = new NetWorkHandel(this);
    m_table_model = new QStandardItemModel(this);
    ui->tableView->setModel(m_table_model);
    //    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList table_h_headers;
    table_h_headers << "填充识别关键字" << "内容";
    m_table_model->setHorizontalHeaderLabels(table_h_headers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    autoRegistration = 2;
    connect(m_net,&NetWorkHandel::finished,this,&chooseALecture::handelNetData);
}

chooseALecture::~chooseALecture()
{
    delete ui;
}

void chooseALecture::addItem(QString name)
{
    ui->comboBox->addItem(name);
}

void chooseALecture::addInfo(QString name, QString time, QString count, QString limit, QString eid,QString status)
{
    QStringList info;
    info << name << time << count << limit << eid << status;
    m_infoList.append(info);
}

void chooseALecture::alarm()
{
    QMessageBox::critical(this, tr("错误信息"),  tr("未查询到讲座信息，请使用微信小程序打开一下讲座，然后重新扫码！"),
                                                    QMessageBox::Default);
}

//void chooseALecture::on_comboBox_currentIndexChanged(int index)
//{
////    if(index == -1)
////        return;
////    QStringList info = m_infoList[index];
////    ui->title_lab->setText(info[0]);
////    ui->start_time_lab->setText(info[1]);
////    ui->count_lab->setText(info[2]);
////    ui->limit_lab->setText(info[3]);
////    ui->eid_lab->setText(info[4]);
////    ui->status_lab->setText(info[5]);
//}


//void chooseALecture::on_pushButton_2_clicked()
//{
//    m_infoList.clear();
//    ui->comboBox->clear();
//    getHistory(m_access_token);
//}


//void chooseALecture::on_pushButton_clicked()
//{
////    emit finished(m_access_token,ui->eid_lab->text(),m_start_time);
////    emit switchWidget();;
//}


void chooseALecture::getHistory(QString access_token)
{
    m_access_token = access_token;
    m_net->getUrl(QUrl("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v1/user/history?access_token=" + access_token));
}

void chooseALecture::setAutoRegistrationStatus(int status)
{
    autoRegistration = status;
}

void chooseALecture::handelNetData(QString jsonData)
{
    //解析json字符串
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QString msgStatus = jsonObj["msg"].toString();
    if("查询成功" == msgStatus)
    {
//        qDebug() << jsonObj;
        if(!jsonObj["data"].toArray().isEmpty())
        {
            qDebug()<< "讲座列表Array:" << jsonObj["data"].toArray();
            for(int i =0;i < jsonObj["data"].toArray().count();++i)
            {
                QJsonObject infoObj = jsonObj["data"].toArray().at(i).toObject();
                QString title = infoObj["title"].toString();
                int statusInt =  infoObj["status"].toInt();
                QString status;
                if(statusInt == 0)
                    status = "未开始";
                else if(statusInt == 1)
                    status = "进行中";
                else if(statusInt == 2)
//                    status = "已结束";
                    continue;
                //讲座人数已满
                if ((infoObj["count"].toInt()) >= (infoObj["limit"].toInt()))
                    continue;
                QString start_time =  QDateTime::fromSecsSinceEpoch(infoObj["start_time"].toVariant().toLongLong()).toString("yyyy-MM-dd hh:mm:ss");
                QString limit = QString::number(infoObj["limit"].toInt());
                QString count =  QString::number(infoObj["count"].toInt());
                QString eid = infoObj["eid"].toString();
//                qDebug() << "eid:" << eid;
                //QString name, QString time, QString count, QString limit, QString eid
                m_start_time = start_time;
//                addInfo(title,start_time,count,limit,eid,status);
                addItem(title);
                eidList.append(eid);
                startTimeList.append(start_time);
            }
            if(ui->comboBox->count() <= 0)
            {
                alarm();
            }
        }
        m_net->getUrl(QUrl(QString("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v1/userinfo?access_token=%1").arg(m_access_token)));
    }
    else if("ok" == msgStatus)
    {
        if(jsonObj["data"].toObject()["extra_info"].isArray())
        {
            QJsonArray ary = jsonObj["data"].toObject()["extra_info"].toArray();
            qDebug()<< "个人信息Array:" << ary;
            for(int i = 0;i< ary.count();++i)
            {
                QString name;
                if(ary.at(i).toObject()["name"].isArray())
                    name = ary.at(i).toObject()["name"].toArray()[0].toString();
                else
                    name = ary.at(i).toObject()["name"].toString();
                QString value = ary.at(i).toObject()["value"].toString();
                if(name == "" || value == "")
                    QMessageBox::critical(this, tr("错误信息"),  tr("程序解析数据出错，或你在报名工具小程序上留的信息有问题，请重新打开程序并检查你在小程序上的信息，如再次出现此问题请发邮件给作者！"),
                                                    QMessageBox::Default);
                m_table_model->setItem(i, 0 , new QStandardItem(name));
                m_table_model->setItem(i, 1 , new QStandardItem(value));
            }
        }
        else
        {
            int user_edit = jsonObj["data"].toObject()["user_edit"].toInt();
            QMessageBox msgBox;
//            qDebug() << "user_edit:" << user_edit;
            if(user_edit == 0 || user_edit == 2)
            {
                if(autoRegistration == 2)
                    msgBox.setText("该讲座允许报名后修改信息且你已设置软件为报名优先(程序默认)，如缺少信息将题目当作内容填入，选择、多选择、下拉框类型将选择第一个报名！如果不同意的话请在软件设置界面取消勾选！");
                else
                    msgBox.setText("该讲座允许报名后修改信息你不允许程序自动帮你填充数据，如缺少信息将弹出输入界面由你输入数据！如允许程序帮你填充数据的话请在软件设置界面勾选！");
            }
            else
            {
                msgBox.setText("该讲座不允许报名后修改信息，如缺少信息将弹出输入界面由你输入数据!");
                emit autoRegistrationSignal(0);
            }
            msgBox.exec();
            emit switchWidget();
            emit finished(m_access_token,eidList[ui->comboBox->currentIndex()],startTimeList[ui->comboBox->currentIndex()],m_data,user_edit);
        }
    }
}

void chooseALecture::on_pushButton_4_clicked()
{
    int row = ui->tableView->model()->rowCount();
    m_table_model->setItem(row, 0 , new QStandardItem("关键字"));
    m_table_model->setItem(row, 1 , new QStandardItem("值"));
}


void chooseALecture::on_pushButton_3_clicked()
{
    for(int i = 0;i< ui->tableView->model()->rowCount();++i)
    {
        QStringList list;
        QString key = m_table_model->data(m_table_model->index(i,0)).toString();
        QString value = m_table_model->data(m_table_model->index(i,1)).toString();
        if(key == "" || value == "")
        {
            QMessageBox::critical(this, tr("错误信息"),  tr("不允许空字符串，请检查信息是否输入完全！"),
                                                    QMessageBox::Default);
            m_data.clear();
            return;
        }
        list.append(key);
        list.append(value);
        m_data.append(list);
    }
    m_net->getUrl(QUrl(QString("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v3/detail?eid=%1&access_token=%2").arg(eidList[ui->comboBox->currentIndex()],m_access_token)));
    qDebug() << "eid:" << eidList[ui->comboBox->currentIndex()];
}

