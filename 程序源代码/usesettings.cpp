#include "usesettings.h"
#include "qdebug.h"
#include "ui_usesettings.h"

useSettings::useSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::useSettings)
{
    ui->setupUi(this);

    ui->checkBox->click();

}

useSettings::~useSettings()
{
    delete ui;
}

void useSettings::on_checkBox_stateChanged(int arg1)
{
//    qDebug() << arg1;
    emit autoRegistration(arg1);
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

