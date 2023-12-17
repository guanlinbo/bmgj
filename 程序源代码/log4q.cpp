#include "log4q.h"

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
//#include<QTextCodec>
#include <iostream>
/**
    qDebug : 调试信息提示
    qWarning: 一般的警告提示
    qCritical: 严重错误提示
    qFatal: 致命错误提示
 */


Log4q::Log4q(QObject *parent): QObject{parent}
{

}

QString Log4q::getPattern(){
    return "[%{time yyyy:MM:dd h:mm:ss.zzz} %{if-debug}DeBug%{endif}%{if-info}Info%{endif}%{if-warning}Warning%{endif}%{if-critical}Critical%{endif}%{if-fatal}Fatal%{endif}] %{file}:%{line} - %{message}" ;
}
void Log4q::userLog(int conOrFile){
    qSetMessagePattern(getPattern());
     //切换日志方式
    switch(conOrFile){
    case 1:
        qInstallMessageHandler(messageOutputConsole);
        break;
    case 2:
        qInstallMessageHandler(messageOutputToFile);
        break;
    default:
        qInstallMessageHandler(messageOutputConsole);
    }

}
void Log4q::messageOutputConsole(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    int msgLevel = 0;
    QString formatMsg = qFormatLogMessage( type, context, msg );
    if( type < msgLevel ) {
        return;
    }
    std::cout << formatMsg.toLocal8Bit().constData() << std::endl;
}
void Log4q::messageOutputToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    QString formatMsg = qFormatLogMessage( type, context, msg );
    QFile file ;
    file.setFileName( "log.txt" );
    file.open( QIODevice::WriteOnly | QIODevice::Append );

    QTextStream outstream;
    outstream.setDevice( &file );
    outstream.setGenerateByteOrderMark(true);
//    outstream.setCodec( QTextCodec::codecForName("utf-8") );

    outstream << formatMsg << "\r\n";
    outstream.flush();
}
