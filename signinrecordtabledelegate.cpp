#include "signinrecordtabledelegate.h"
#include "qdebug.h"
#include <QPushButton>
#include <QMessageBox>
signInRecordTableDelegate::signInRecordTableDelegate(QWidget *parent)
    : QStyledItemDelegate{parent}
{}

QWidget *signInRecordTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column()==9){
        auto widget = new QPushButton(parent);
        widget->setText("删除");
        int recordId = index.data().toInt();
        widget->setProperty("recordId",recordId);
        widget->setProperty("row",index.row());
        connect(widget,&QPushButton::clicked,this,&signInRecordTableDelegate::onDeleteButtonClick);
        return widget;
    }
    else{
        return QStyledItemDelegate::createEditor(parent,option,index);
    }
}

void signInRecordTableDelegate::onDeleteButtonClick()
{
    QPushButton* button = (QPushButton*)sender();
    int recordId = -1;
    recordId = button->property("recordId").toInt();
    int row = button->property("row").toInt();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(button, "确认删除？", QString("确认删除 recordId为 %1 的签到记录？").arg(recordId),QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit deleteEnsureButtonClickedSignal(recordId,row);
    }
    else{
        qDebug()<<"取消删除 recordId:"<<recordId;
    }
}
