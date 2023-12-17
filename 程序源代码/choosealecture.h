#ifndef CHOOSEALECTURE_H
#define CHOOSEALECTURE_H

#include "networkhandel.h"
#include <QWidget>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QStandardItemModel>
namespace Ui {
class chooseALecture;
}

class chooseALecture : public QWidget
{
    Q_OBJECT

public:
    explicit chooseALecture(QWidget *parent = nullptr);
    ~chooseALecture();
    void addItem(QString name);
    void addInfo(QString name, QString time, QString count, QString limit, QString eid, QString status);
    void alarm();
private slots:
//    void on_comboBox_currentIndexChanged(int index);

//    void on_pushButton_2_clicked();
//    void on_pushButton_clicked();
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

public slots:
    void handelNetData(QString jsonData);
    void getHistory(QString access_token);
    void setAutoRegistrationStatus(int status);
signals:
    void retrieveInfo();
    void finished(QString,QString,QString,QVector<QStringList>,int);
    void switchWidget();
    void autoRegistrationSignal(int status);
private:
    Ui::chooseALecture *ui;
    QVector<QStringList> m_infoList;
    NetWorkHandel* m_net;
    QString m_access_token;
    QString m_start_time;
    QStandardItemModel* m_table_model;
    QStringList eidList;
    QStringList startTimeList;
    QVector<QStringList> m_data;
    int autoRegistration;
};

#endif // CHOOSEALECTURE_H
