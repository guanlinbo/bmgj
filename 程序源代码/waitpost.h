#ifndef WAITPOST_H
#define WAITPOST_H

#include <QWidget>
#include<QTimer>
#include"networkhandel.h"
#include <QThread>
#include "multiplechoice.h"
#include "settings.h"
namespace Ui {
class waitPost;
}

class waitPost : public QWidget
{
    Q_OBJECT

public:
    explicit waitPost(QWidget *parent = nullptr);
    ~waitPost();
public slots:
    void GetDateTime();
    void setStartTime(QString time);
    void StartGrabbing(QString token, QString eid, QString time, QVector<QStringList> data);
public slots:
    void handelNetData(QString jsonData);
    void setAutoRegistrationStatus(int status);
    void setParityRequirement(int status);

    bool isNum(QString str);
    bool isInt(QString str);
private:
    Ui::waitPost *ui;
    QTimer* myTimer;
    QString start_time;
    QJsonObject m_infoObj;
    NetWorkHandel* m_net;
    bool isSuccess;
    bool isGetInfo;
    QString m_token;
    QString m_eid;
    QVector<QStringList>mapObj;
    QStringList keyList;
    QStringList valueList;
    bool autoRegistration;
    int parityRequirement;          //0无要求 1奇数 2偶数
    bool isGetCount;
    bool programFilling;
    settings m_set;
};

#endif // WAITPOST_H
