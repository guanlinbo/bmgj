#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QPixmap>
#include<QTimer>
#include"networkhandel.h"

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void onTimeOut();
    void handelNetData(QString data);
    void on_pushButton_clicked();

signals:
    void finished(QString);
    void switchWidget();
private:
    Ui::login *ui;
    QString m_code;
    QTimer *tim;
    NetWorkHandel* m_net;

};

#endif // LOGIN_H
