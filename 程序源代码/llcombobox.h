#ifndef LLCOMBOBOX_H
#define LLCOMBOBOX_H

#include <QComboBox>

class QListWidgetItem;
class LLComboBoxPrivate;
class LLComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit LLComboBox(QWidget* parent = NULL);

    /**
     * @brief addDataItem 添加数据
     * @param text
     * @param userData
     */
    void addDataItem(const QString &text, const QVariant &userData = QVariant());

    /**
     * @brief setSelectedData 设置选中的数据
     * @param selectedData
     */
    void setSelectedData(const QStringList &selectedData);

    /**
     * @brief setPopupViewHeight 设置下拉列表弹窗的高度，默认值100
     * @param height
     */
    void setPopupViewHeight(int height);

    /**
     * @brief selectedDataText 获取选择的数据
     * @return
     */
    QStringList selectedDataText();

    /**
     * @brief selectedUserData 获取选择数据对应的用户数据
     * @return
     */
    QList<QVariant> selectedUserData();

private slots:
    void slot_itemChanged(QListWidgetItem *item);
private:

    LLComboBoxPrivate *m_pd;
};

#endif // LLCOMBOBOX_H

