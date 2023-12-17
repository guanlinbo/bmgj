#include "mainwindow.h"

#include <QApplication>
#include<QSurfaceFormat>
#include"log4q.h"
int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(4, 3);
    Log4q::userLog(Log4q::FILE);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
