#ifndef LEAVEANDMAKEUPTABLEDELEGATE_H
#define LEAVEANDMAKEUPTABLEDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>

class LeaveAndMakeUpTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit LeaveAndMakeUpTableDelegate(int whichType,QWidget *parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private slots:
    void on_removeButtonClick();
private:
    int whichType;

signals:
    void removeButtonClicked(int whichType,int row);
};

#endif // LEAVEANDMAKEUPTABLEDELEGATE_H
