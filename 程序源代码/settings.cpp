#include "settings.h"

settings::settings()
{
    m_setting = new QSettings("setting.ini",QSettings::IniFormat);
    m_setting->beginGroup("SETTING");
}

bool settings::isshowinfo()
{
    QString noshowinfo = m_setting->value("noshowinfo").toString();
    if(noshowinfo.isEmpty())
    {
        noshowinfo = "yes";
        m_setting->setValue("noshowinfo",noshowinfo);
        return true;
    }
    else if(noshowinfo == "yes")
    {
        return true;
    }
    else
    {
      return false;
    }
}

bool settings::isautoregistration()
{
    QString registration = m_setting->value("autoregistration").toString();
    if(registration.isEmpty())
    {
        registration = "yes";
        m_setting->setValue("autoregistration",registration);
    }
    else if(registration == "yes")
    {
        return true;
    }
    else
    {
        return false;
    }

}
