#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"networkhandel.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void getUnid(QString token, QString eid, QString start_time);
    void handelNetData(QString jsonData);
//    void handelStart(QVector<QStringList> data);
private slots:
    void on_EquipmentList_clicked();
    void on_RemoteControl_clicked();

    void on_plusSet_clicked();

    void on_plusSet_2_clicked();

private:
    Ui::MainWindow *ui;
    NetWorkHandel* m_net;
    QString m_token;
    QString m_eid;
    QString m_unid;
};
#endif // MAINWINDOW_H
