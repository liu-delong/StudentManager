#include "leaveandmakeuptabledelegate.h"
#include <QPushButton>
LeaveAndMakeUpTableDelegate::LeaveAndMakeUpTableDelegate(int whichType,QWidget *parent)
    : QStyledItemDelegate{parent}
{
    this->whichType = whichType;
}

QWidget *LeaveAndMakeUpTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    int row = index.row();
    auto widget = new QPushButton(parent);
    widget->setText("删除");
    widget->setProperty("row",row);
    connect(widget,&QPushButton::clicked,this,&LeaveAndMakeUpTableDelegate::on_removeButtonClick);
    return widget;
}
void LeaveAndMakeUpTableDelegate::on_removeButtonClick()
{
    QPushButton* button = (QPushButton*)sender();
    int row = button->property("row").toInt();
    emit removeButtonClicked(whichType,row);
}
