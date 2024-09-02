#ifndef SIGNINTABLEDELEGATE_H
#define SIGNINTABLEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QList>
#include <QPushButton>
#include <QDateEdit>

class SignInTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit SignInTableDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    enum ColumnType{
        Button,
        Default,
        CheckBox,
        SpinBox,
        ComboBox,
        DateEdit
    };
    void setColumnType(QList<ColumnType> types);
    //设置第column列的comboBox的可选项，column的索引从1开始。
    void setComboBoxMap(int column,QMap<QString,QVariant> map);
private:
    QList<ColumnType> columntypes;
    QMap<int,QMap<QString,QVariant>> ComboBoxMaps;


signals:
    void buttonClicked(QPushButton* button) const;
};

#endif // SIGNINTABLEDELEGATE_H
