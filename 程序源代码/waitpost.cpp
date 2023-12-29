#include "waitpost.h"
#include "qjsonobject.h"
#include "ui_waitpost.h"
#include<QDateTime>
#include<QMessageBox>
#include <synchapi.h>
#include<QInputDialog>
#include"settings.h"
waitPost::waitPost(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waitPost)
{
    ui->setupUi(this);
    myTimer = new QTimer(this);
    m_net = new NetWorkHandel(this);
    isSuccess = true;
    isGetInfo = true;
    isGetCount = false;
    autoRegistration = true;
    programFilling = false;
    connect(m_net,&NetWorkHandel::finished,this,&waitPost::handelNetData);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(GetDateTime()));
}

waitPost::~waitPost()
{
    delete ui;
}

void waitPost::GetDateTime()
{
    QDateTime dateTime= QDateTime::currentDateTime();
    QString end_str = dateTime.toString("yyyy-MM-dd hh:mm:ss");

    QDateTime time1 = QDateTime::fromString(start_time, "yyyy-MM-dd hh:mm:ss");

    QDateTime time2 = QDateTime::fromString(end_str, "yyyy-MM-dd hh:mm:ss");

    int min = time2.secsTo(time1);
    if(min <= 2)
    {
        if(isGetInfo == true)
        {
            qDebug() << "isGetInfo";
            m_net->getUrl(QUrl(QString("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v1/req_detail?access_token=%1&eid=%2").arg(m_token,m_eid)));
        }
        else if(isGetCount == true)
        {
            qDebug() << "isGetCount";
            m_net->getUrl(QUrl(QString("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v3/detail?eid=%1&access_token=%2").arg(m_eid,m_token)));
        }
    }
    //天数计算
    int days = (min)/(24*3600);

    //小时计算
    int hours = (min)%(24*3600)/3600;

    //分钟计算
    int minutes = (min)%3600/60;

    //秒计算
    int second = (min)%60;

    ui->label_6->setText(QString::number(days) + "天" + QString::number(hours)  + "小时"+ QString::number(minutes)  + "分钟" + QString::number(second) + "秒");
    ui->current_time->setText(end_str);
}

void waitPost::setStartTime(QString time)
{
    ui->start_time->setText(time);
    start_time = time;
    myTimer->start(500);
}

void waitPost::StartGrabbing(QString token, QString eid, QString time, QVector<QStringList> data)
{
    //    m_infoObj = obj;
    //    isSuccess = false;
    //    qDebug() << time;
    m_token = token;
    m_eid = eid;
    mapObj = data;
    for(int i = 0;i<mapObj.count();++i)
    {
        keyList.append(mapObj[i][0]);
        valueList.append(mapObj[i][1]);
    }
    //    qDebug()<< "mapObj:" << data[0][0];
    ui->start_time->setText(time);
    start_time = time;
    qDebug() << "t:" << time;
    myTimer->start(500);
}


void waitPost::handelNetData(QString jsonData)
{
    if(m_set.isautoregistration())
        autoRegistration = true;
    else
        autoRegistration = false;
//    //解析json字符串
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QString msgStatus = jsonObj["msg"].toString();
    QJsonArray arry = jsonObj["data"].toObject()["req_info"].toArray();
    qDebug() << arry;
    qDebug() << msgStatus;
    //构造json
    if("" == msgStatus)
    {
        qDebug() << "报名成功!";
        QDateTime dateTime= QDateTime::currentDateTime();
        QString end_str = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        ui->label_7->setText("报名成功! " + end_str);
        if(programFilling == true)
        {
            if(m_set.isshowinfo())
                QMessageBox::critical(this, tr("告警信息"),  tr("请注意，本次报名数据有部分为程序帮你填写，请前往小程序核对并修改信息！"),
                                                    QMessageBox::Default);
        }
        isSuccess = true;
        isGetCount = false;
        return;
    }
    else if("报名未开始" == msgStatus)
    {
        Sleep(300);
        m_net->SendJsonData(m_infoObj);
        return;
    }
    else if("请求过于频繁，请稍后重试" == msgStatus)
    {
        Sleep(700);
        m_net->SendJsonData(m_infoObj);
        return;
    }
    else if("ok" == msgStatus)
    {
        if(isGetInfo == true)
        {
            if(!arry.isEmpty())
            {
//                qDebug() << jsonObj;

                QJsonArray jsonArray;
                QJsonArray kongarray;
                QJsonObject likeObject1;
                likeObject1.insert("access_token",m_token);
                likeObject1.insert("eid",m_eid);

                isGetInfo = false;
                bool Success = true;
//                qDebug() << arry;
                for(int i = 0;i < arry.count();++i)
                {
                    QJsonObject obj = arry.at(i).toObject();
                    QString field_name = obj["field_name"].toString();
                    int field_type = obj["field_type"].toInt();
                    QString field_key = obj["field_key"].toString();
                    QString type_text = obj["type_text"].toString();
                    QJsonObject infoobj;
                    infoobj.insert("field_name",field_name);
                    bool isInt;
                    field_key.toInt(&isInt);
                    if(isInt)
                        infoobj.insert("field_key",field_key.toInt());
                    else
                        infoobj.insert("field_key",field_key);
                    if(field_key == "")
                        infoobj.insert("field_key",obj["field_key"].toInt());
        //            qDebug() <<field_name;
                    bool flag = false;
                    if(field_type == 0 || field_type == 13  || type_text == "手机号" || type_text == "数字")
                    {
                        bool bOk = false;
                        for(int j = 0;j< keyList.count();++j)
                        {
                            if(field_name.contains(keyList.at(j)))
                            {
                                if(keyList.at(j) == "")break;
                                qDebug() << "valueList.at(j))" << valueList.at(j) << "keyList.at(j)" << keyList.at(j);
                                flag = true;
                                infoobj.insert("field_value",valueList.at(j));
                                jsonArray.append(infoobj);
                                break;
                            }
                        }
                        if(flag == false)
                        {
                            if(autoRegistration == true)
                            {
                                programFilling = true;
                                flag = true;
                                qDebug() <<  "field_name:" << field_name;
                                infoobj.insert("field_value",field_name);
                                jsonArray.append(infoobj);
                            }
                            else
                            {
                                QString sName = QInputDialog::getText(this,
                                                                      "输入界面",
                                                                      QString("请输入 %1").arg(field_name),
                                                                      QLineEdit::Normal,
                                                                      "张三",
                                                                      &bOk
                                                                      );
                                if (bOk && !sName.isEmpty())
                                {
                                    flag = true;
                                    infoobj.insert("field_value",sName);
                                    jsonArray.append(infoobj);
                                }
                            }
                        }
                    }
                    else if(field_type == 4 || field_type == 10)
                    {
                        bool bOk = false;
                        QStringList tLi;

                        QJsonArray new_options;
                        if(field_type == 4)
                            new_options = obj["new_options"].toArray();
                        else
                            new_options = obj["options"].toArray();
                        for(int j = 0;j< new_options.count();++j)
                        {
                            QString str;
                            if(field_type == 4)
                                str = new_options.at(j).toObject()["value"].toString();
                            else
                                str = new_options.at(j).toString();
                            tLi << str;
                            if(isNum(str))
                                goto nameGoto;
                            if(valueList.filter(str).count() != 0)
                            {
                                flag = true;
                                infoobj.insert("field_value",str);
                                jsonArray.append(infoobj);
                                break;
                            }
                            nameGoto:
                            for(int j =0;j< valueList.count();++j)
                            {
                                if (str.contains(valueList[j]))
                                {
                                    flag = true;
                                    infoobj.insert("field_value",str);
                                    jsonArray.append(infoobj);
                                    break;
                                }
                            }
                        }
                        if(flag == false)
                        {
                            if(autoRegistration == true)
                            {
                                QString str;
                                if(field_type == 4)
                                    str = new_options.at(0).toObject()["value"].toString();
                                else
                                    str = new_options.at(0).toString();
                                programFilling = true;
                                flag = true;
                                infoobj.insert("field_value",str);
                                jsonArray.append(infoobj);
                            }
                            else
                            {
                                QString sGender = QInputDialog::getItem(this,
                                    "输入界面",
                                    QString("请选择 %1").arg(field_name),
                                                                 tLi,
                                                             0,
                                                             false,
                                                             &bOk);
                                    if (bOk && !sGender.isEmpty())
                                    {
                                        //                        ui->label_Gender->setText(sGender);
                                        flag = true;
                                        infoobj.insert("field_value",sGender);
                                        jsonArray.append(infoobj);
                                    }
                            }
                        }
                    }
                    else if(type_text == "多项选择" || field_type == 5)
                    {
                        QJsonArray fieldAry;
                        QJsonArray new_fieldAry;
                        QJsonArray valueAry;
                        if(autoRegistration == true)
                        {
                            QString str;
                            QString key;
                            str = obj["new_options"].toArray().at(0).toObject()["value"].toString();
                            key = obj["new_options"].toArray().at(0).toObject()["key"].toString();
                            programFilling = true;
                            flag = true;
                            fieldAry.append(str);
                            new_fieldAry.append(key);
                        }
                        else
                        {
                            QJsonArray mcary = obj["new_options"].toArray();
                            multipleChoice* mc = new multipleChoice;
                            mc->setModal(true);
                            QMap<QString,QString>map;
                            for(int i =0;i< mcary.count();++i)
                            {
                                mc->addDataItem(mcary.at(i).toObject()["value"].toString());
                                map[mcary.at(i).toObject()["value"].toString()] = mcary.at(i).toObject()["key"].toString();
                            }
                            QStringList selectlist;
                            selectlist << mcary.at(0).toObject()["value"].toString();
                            mc->setSelectedData(selectlist);
                            connect(mc,&multipleChoice::selectedDataTextSignal,this,[&](QStringList list){
                                for(int i = 0;i< list.count();++i)
                                {
                                    fieldAry.append(list.at(i));
                                    new_fieldAry.append(map[list.at(i)]);
                                }
                                mc->hide();
                                mc->close();
                            });
                            mc->setWindowTitle("多项选择  请选择:" + field_name);
                            // mc->showmcPopup();
                            mc->exec();
                            if(fieldAry.count() == 0)
                                flag = false;
                            else
                                flag = true;
                            // QString sGender = QInputDialog::getItem(this,
                            //                                         "输入界面",
                            //                                         QString("请选择 %1").arg(field_name),
                            //                                         tLi,
                            //                                         0,
                            //                                         false,
                            //                                         &bOk);
                            // if (bOk && !sGender.isEmpty())
                            // {
                            //     //                        ui->label_Gender->setText(sGender);
                            //     flag = true;
                            //     infoobj.insert("field_value",sGender);
                            //     jsonArray.append(infoobj);
                            // }

                        }

                        infoobj.insert("field_value",fieldAry);
                        infoobj.insert("new_field_value",new_fieldAry);
                        jsonArray.append(infoobj);
                    }
                    if(flag == false)
                    {
                        Success = false;
                        QString prompt = QString("未找到关于[%1] 相关的关键字信息，报名取消！").arg(field_name);
                        QMessageBox::critical(this, tr("错误信息"),  tr(prompt.toStdString().c_str()),
                                                            QMessageBox::Default);
                        break;
                    }
                }
                if(Success == false)
                    return;
                likeObject1.insert("info",jsonArray);
                likeObject1.insert("on_behalf",0);
                likeObject1.insert("items",kongarray);
                likeObject1.insert("referer","");
                likeObject1.insert("fee_type","");
                if(parityRequirement == 0)
                {
                    m_net->SendJsonData(likeObject1);
                }
                m_infoObj = likeObject1;
                qDebug() << likeObject1;
            }
        }
        else if(isGetCount == true)
        {
            int conut =  jsonObj["data"].toObject()["count"].toInt();
            if(parityRequirement == 1)
            {
                if(conut % 2 == 0)
                {
                    m_net->SendJsonData(m_infoObj);
                }
            }
            else if(parityRequirement == 2)
            {
                if(conut % 2 == 1)
                {
                    m_net->SendJsonData(m_infoObj);
                }
            }
        }
    }
    else
    {
        isGetCount = false;
        ui->label_7->setText("错误信息：" +  msgStatus);
    }
//    qDebug() << jsonObj;
    QDateTime dateTime= QDateTime::currentDateTime();//获取系统当前的时间

    QString str = dateTime .toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << str;
//    else
//    {
//        ui->label_7->setText("错误信息：" +  msgStatus);
    //    }
}

void waitPost::setAutoRegistrationStatus(int status)
{
    // if(2 == status)
    // {
    //     autoRegistration = true;
    // }
    // else
    // {
    //     autoRegistration = false;
    // }
}

void waitPost::setParityRequirement(int status)
{
    if(status != 0)
        isGetCount = true;
    parityRequirement = status;
}


// 判断是否为数字
bool waitPost::isNum(QString str)
{
    bool isNum;
    str.toDouble(&isNum);
    return isNum;
}

// 判断是否为整数
bool waitPost::isInt(QString str)
{
    bool isInt;
    str.toInt(&isInt);
    return isInt;
}
