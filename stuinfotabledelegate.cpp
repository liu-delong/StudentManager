#include "stuinfotabledelegate.h"
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <stuinfo.h>
StuInfoTableDelegate::StuInfoTableDelegate(QWidget *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget *StuInfoTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column()==StuInfo::MODIFY){
        //qDebug()<<index.column()<<index.data();
        QPushButton* widget = new QPushButton(parent);
        widget->setText("修改");
        widget->setProperty("stu_id",index.data());
        connect(widget,&QPushButton::clicked,this,&StuInfoTableDelegate::onModifyButtonClick);

        return widget;
    }
    else{
        return QStyledItemDelegate::createEditor(parent,option,index);
    }
}

void StuInfoTableDelegate::onModifyButtonClick()
{
    QPushButton* widget = (QPushButton*)sender();
    int stu_id = widget->property("stu_id").toInt();
    emit ModifyButttonClickSignal(stu_id);
}
