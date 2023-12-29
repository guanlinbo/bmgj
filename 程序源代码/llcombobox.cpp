#include <QListWidget>
#include <QCheckBox>
#include <QList>
#include <QLineEdit>
#include <QDebug>

#include "llcombobox.h"

class LLComboBoxPrivate
{
public:

    QStringList selectedDataList;
    QListWidget *pListWidget;
};

LLComboBox::LLComboBox(QWidget *parent)
    : QComboBox(parent)
    , m_pd(new LLComboBoxPrivate)
{
    m_pd->pListWidget = new QListWidget;
    m_pd->pListWidget->setFixedHeight(100);
    connect(m_pd->pListWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(slot_itemChanged(QListWidgetItem *)));
    setView(m_pd->pListWidget);
    //设置Editable为true，lineEdit() 函数才不返回空
    setEditable(true);
}

void LLComboBox::addDataItem(const QString &text, const QVariant &userData)
{
    m_pd->pListWidget->blockSignals(true);
    QListWidgetItem *pItem = new QListWidgetItem(text, m_pd->pListWidget);
    pItem->setData(Qt::UserRole, userData);
    pItem->setCheckState(Qt::Unchecked);
    //设置QListWidgetItem 可交互且可以选中和取消选中
    pItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    QComboBox::addItem(text);
    QComboBox::setCurrentText("");
    m_pd->pListWidget->blockSignals(false);
}

void LLComboBox::setSelectedData(const QStringList &selectedData)
{
    m_pd->selectedDataList.clear();
    m_pd->selectedDataList += selectedData;

    QString text = "";
    for (int var = 0; var < m_pd->selectedDataList.size(); ++var)
    {
        m_pd->pListWidget->blockSignals(true);
        QListWidgetItem *pItem = m_pd->pListWidget->item(var);
        pItem->setCheckState(Qt::Checked);
        m_pd->pListWidget->blockSignals(false);

        text.append(m_pd->selectedDataList[var]);
        if(var < m_pd->selectedDataList.size() - 1)
        {
            text.append(",");
        }
    }

    lineEdit()->setText(text);
}

QStringList LLComboBox::selectedDataText()
{
    return m_pd->selectedDataList;
}

QList<QVariant> LLComboBox::selectedUserData()
{
    QList<QVariant> dataList;
    for (int var = 0; var < m_pd->selectedDataList.size(); ++var)
    {
        QListWidgetItem *pItem = m_pd->pListWidget->item(var);
        if(pItem->checkState() == Qt::Checked)
        {
            dataList.append(pItem->data(Qt::UserRole));
        }
    }

    return dataList;
}

void LLComboBox::slot_itemChanged(QListWidgetItem *item)
{
    qDebug() << QString("text=") << item->text();
    if(m_pd->selectedDataList.contains(item->text()))
    {
        m_pd->selectedDataList.removeOne(item->text());
    }
    else
    {
        m_pd->selectedDataList.append(item->text());
    }

    QString text = "";
    for (int var = 0; var < m_pd->selectedDataList.size(); ++var)
    {
        text.append(m_pd->selectedDataList[var]);
        if(var < m_pd->selectedDataList.size() - 1)
        {
            text.append(",");
        }
    }

    lineEdit()->setText(text);
}

