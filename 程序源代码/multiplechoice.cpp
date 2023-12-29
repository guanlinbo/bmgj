#include "multiplechoice.h"
#include "qdebug.h"
#include "ui_multiplechoice.h"

multipleChoice::multipleChoice(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::multipleChoice)
{
    ui->setupUi(this);
}

multipleChoice::~multipleChoice()
{
    delete ui;
}


void multipleChoice::addDataItem(const QString &text, const QVariant &userData)
{
    ui->comboBox->addDataItem(text,userData);
}

void multipleChoice::setSelectedData(const QStringList &selectedData)
{
    ui->comboBox->setSelectedData(selectedData);
}

void multipleChoice::showmcPopup()
{
    ui->comboBox->showPopup();
}

void multipleChoice::on_pushButton_clicked()
{
    emit selectedDataTextSignal(ui->comboBox->selectedDataText());
}

