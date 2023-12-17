#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    tim = new QTimer(this);
    m_net = new NetWorkHandel(this);
    connect(m_net,&NetWorkHandel::finished,this,&login::handelNetData);
    connect(tim,SIGNAL(timeout()),this,SLOT(onTimeOut()));

    m_net->getUrl(QUrl("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll_web/v1/pc_code"));
}

login::~login()
{
    delete ui;
}


void login::onTimeOut()
{
    m_net->getUrl(QUrl("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll_web/v1/pc_login?code=" + m_code));
}

void login::handelNetData(QString data)
{
    //解析json字符串
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    QString msgStatus = jsonObj["msg"].toString();
    if("ok" == msgStatus)
    {
        if(tim->isActive())
        {
            tim->stop();
            ui->label_2->setText("扫码成功!");
            QString access_token = jsonObj["data"].toObject()["access_token"].toString();
            emit finished(access_token);
            emit switchWidget();
            qDebug() << "access_token:" << access_token;
        }
        else
        {
            QString base64 = jsonObj["data"].toObject()["qrcode"].toString();
            QPixmap image;
            image.loadFromData(QByteArray::fromBase64(base64.section(",", 1).toLocal8Bit()));
            ui->label->setPixmap(image);

            QString code = jsonObj["data"].toObject()["code"].toString();
            m_code = code;
            //1秒定时器判断有没有扫描二维码
            tim->start(1000);
        }
    }
}

void login::on_pushButton_clicked()
{
    m_net->getUrl(QUrl("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll_web/v1/pc_code"));
    ui->label_2->setText("等待扫码中!");
}

