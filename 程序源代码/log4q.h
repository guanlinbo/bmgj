#ifndef LOG4Q_H
#define LOG4Q_H

#include <QObject>

class Log4q : public QObject
{
    Q_OBJECT
public:
    explicit Log4q(QObject *parent = nullptr);
    static const int CONSOLE = 1;
    static const int FILE = 2;
    static QString getPattern();
    static void userLog(int conOrFile);
    static void messageOutputConsole(QtMsgType type,
                                     const QMessageLogContext& context,const QString& msg);
    static void messageOutputToFile(QtMsgType type,
                                    const QMessageLogContext& context,const QString& msg);
signals:

};

#endif // LOG4Q_H
