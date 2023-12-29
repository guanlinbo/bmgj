#ifndef MULTIPLECHOICE_H
#define MULTIPLECHOICE_H

#include "qvariant.h"
#include <QDialog>

namespace Ui {
class multipleChoice;
}

class multipleChoice : public QDialog
{
    Q_OBJECT

public:
    explicit multipleChoice(QWidget *parent = nullptr);
    ~multipleChoice();
    void addDataItem(const QString &text, const QVariant &userData = QVariant());
    void setSelectedData(const QStringList &selectedData);
    void showmcPopup();
private slots:

    void on_pushButton_clicked();

signals:
    void selectedDataTextSignal(QStringList);
private:
    Ui::multipleChoice *ui;
};

#endif // MULTIPLECHOICE_H
