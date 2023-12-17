#ifndef USESETTINGS_H
#define USESETTINGS_H

#include <QWidget>

namespace Ui {
class useSettings;
}

class useSettings : public QWidget
{
    Q_OBJECT

public:
    explicit useSettings(QWidget *parent = nullptr);
    ~useSettings();
signals:
    void autoRegistration(int status);
    void parityRequirement(int status);
    void nextSignal();
private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:
    Ui::useSettings *ui;
};

#endif // USESETTINGS_H
