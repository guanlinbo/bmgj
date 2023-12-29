#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "qsettings.h"
class settings
{
public:
    settings();
    QSettings* m_setting;
    bool isshowinfo();
    bool isautoregistration();
};

#endif // SETTINGS_H
