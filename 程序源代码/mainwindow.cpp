#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QBuffer>
#include<QGraphicsDropShadowEffect>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_net = new NetWorkHandel(this);
    connect(m_net,&NetWorkHandel::finished,this,&MainWindow::handelNetData);
    connect(ui->loginPage,&login::finished,ui->chooseALecturePage,&chooseALecture::getHistory);
    connect(ui->loginPage,&login::switchWidget,this,&MainWindow::on_EquipmentList_clicked);
    connect(ui->chooseALecturePage,&chooseALecture::finished,ui->waitPostPage,&waitPost::StartGrabbing);
    connect(ui->chooseALecturePage,&chooseALecture::switchWidget,this,&MainWindow::on_plusSet_clicked);
    connect(ui->chooseALecturePage,&chooseALecture::autoRegistrationSignal,ui->waitPostPage,&waitPost::setAutoRegistrationStatus);
    connect(ui->useSettingsPage,&useSettings::autoRegistration,ui->chooseALecturePage,&chooseALecture::setAutoRegistrationStatus);
    connect(ui->useSettingsPage,&useSettings::autoRegistration,ui->waitPostPage,&waitPost::setAutoRegistrationStatus);
    connect(ui->useSettingsPage,&useSettings::parityRequirement,ui->waitPostPage,&waitPost::setParityRequirement);
    connect(ui->useSettingsPage,&useSettings::nextSignal,this,&MainWindow::on_RemoteControl_clicked);

//    connect(ui->registerLecturesPage,&registerLectures::finished,this,&MainWindow::handelStart);


    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);          //设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(Qt::gray);       //设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(20);        //设定阴影的圆角大小
    ui->frame->setGraphicsEffect(effect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getUnid(QString token, QString eid,QString start_time)
{
    m_token = token;
    m_eid = eid;
    m_net->getUrl(QUrl("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v1/userinfo?access_token=" + token));
    ui->waitPostPage->setStartTime(start_time);
}

void MainWindow::handelNetData(QString jsonData)
{
    //解析json字符串
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QString msgStatus = jsonObj["msg"].toString();
    if("ok" == msgStatus)
    {
        QString unid = jsonObj["data"].toObject()["unionid"].toString();
        qDebug() << "unid:" << unid;
        //QString token,QString eid,QString unid
//        ui->registerLecturesPage->startGetInfo(m_token,m_eid,unid);
    }
}

//void MainWindow::handelStart(QVector<QStringList>data)
//{
////    ui->stackedWidget->setCurrentWidget(ui->waitPostPage);
////    ui->waitPostPage->StartGrabbing(data);
//}



void MainWindow::on_EquipmentList_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->chooseALecturePage);
}


void MainWindow::on_RemoteControl_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
}


void MainWindow::on_plusSet_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->waitPostPage);
}


void MainWindow::on_plusSet_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->useSettingsPage);
}

