#include "networkhandel.h"
#include "qjsonobject.h"

NetWorkHandel::NetWorkHandel(QObject *parent)
    : QObject{parent}
{
    naManager = new QNetworkAccessManager(this);
    connect(naManager, &QNetworkAccessManager::finished, this, &NetWorkHandel::receiveReplyGet);
}

void NetWorkHandel::getUrl(QUrl url)
{
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json;charset=utf-8"));
    QNetworkReply *reply = naManager->get(request);    //get请求头

    //开启事件循环，直到请求完成
    QEventLoop loop;
    connect(reply,&QNetworkReply::finished,&loop,&QEventLoop::quit);
    loop.exec();
}

void NetWorkHandel::SendJsonData(QJsonObject obj)
{
    QJsonDocument jsonDoc = QJsonDocument(obj);
    QByteArray post_data = jsonDoc.toJson(QJsonDocument::Compact);
    request.setUrl(QUrl("http://api-xcx-qunsou.weiyoubot.cn/xcx/enroll/v5/enroll"));
    naManager->post(request,post_data);
}

void NetWorkHandel::receiveReplyGet(QNetworkReply *reply)
{
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();
    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();

    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError)
    {
        qDebug() << "Failed: " << reply->errorString();
    }
    else
    {
        // 获取返回内容
        QString reply_data = reply->readAll();
        emit finished(reply_data);
    }
}
