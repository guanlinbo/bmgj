#ifndef NETWORKHANDEL_H
#define NETWORKHANDEL_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>

class NetWorkHandel : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkHandel(QObject *parent = nullptr);
    void getUrl(QUrl url);
     void SendJsonData(QJsonObject obj);
signals:
    void finished(QString);

private slots:
    void receiveReplyGet(QNetworkReply *reply);

private:
    QNetworkRequest request;
    QNetworkAccessManager* naManager;

};

#endif // NETWORKHANDEL_H
