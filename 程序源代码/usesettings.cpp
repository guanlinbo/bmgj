#include "usesettings.h"
#include "ui_usesettings.h"

useSettings::useSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::useSettings)
{
    ui->setupUi(this);

    settings = new QSettings("setting.ini",QSettings::IniFormat);
    settings->beginGroup("SETTING");
    QString noshowinfo = settings->value("noshowinfo").toString();
    if(noshowinfo.isEmpty())
    {
        noshowinfo = "yes";
        settings->setValue("noshowinfo",noshowinfo);
        ui->checkBox_2->click();
    }
    else if(noshowinfo == "yes")
        ui->checkBox_2->click();

    QString registration = settings->value("autoregistration").toString();
    if(registration.isEmpty())
    {
        registration = "yes";
        settings->setValue("autoregistration",registration);
        ui->checkBox->click();
    }
    else if(registration == "yes")
        ui->checkBox->click();

}

useSettings::~useSettings()
{
    delete ui;
}

void useSettings::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 2)
        settings->setValue("autoregistration","yes");
    else if(arg1 == 0)
        settings->setValue("autoregistration","no");
//    qDebug() << arg1;
    // emit autoRegistration(arg1);
}


void useSettings::on_comboBox_currentIndexChanged(int index)
{
//    qDebug() << index;
    emit parityRequirement(index);
}


void useSettings::on_pushButton_clicked()
{
    emit nextSignal();
}


void useSettings::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1 == 2)
        settings->setValue("noshowinfo","yes");
    else if(arg1 == 0)
        settings->setValue("noshowinfo","no");
}

