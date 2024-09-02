#include "signintabledelegate.h"
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QDebug>
#include <QComboBox>
SignInTableDelegate::SignInTableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}
QWidget *SignInTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ColumnType type = columntypes[index.column()];
    if(type == Button){
        auto editor = new QPushButton(parent);
        editor->setProperty("row",index.row());
        editor->setProperty("type",type);
        editor->setText(QString("签到"));
        connect(editor,&QPushButton::clicked,this,[this,editor](){
            emit buttonClicked(editor);
        });

        return editor;
    }
    else if(type == CheckBox){
        auto editor = new QCheckBox(parent);
        editor->setProperty("type",type);
        return editor;
    }
    else if(type == SpinBox){
        auto editor = new QSpinBox(parent);
        editor->setProperty("type",type);
        return editor;
    }
    else if (type == ComboBox){
        auto editor = new QComboBox(parent);
        editor->setProperty("type",type);
        auto comboBoxMap = ComboBoxMaps.value(index.column()+1);
        for(auto iter = comboBoxMap.constBegin();iter!=comboBoxMap.constEnd();++iter){
            QString k = iter.key();
            QVariant v = iter.value();
            editor->addItem(k,v);
        }
        return editor;
    }
    else if (type == DateEdit){
        auto editor = new QDateEdit(parent);
        editor->setProperty("type",type);
        return editor;
    }
    else{
        return QStyledItemDelegate::createEditor(parent,option,index);
    }
}
QColor getColor(int acount){
    if(acount == 0){
        return QColor(0,255,0,100);
    }
    else if(acount == 1){
        return QColor(255,255,0,100);
    }
    else if(acount==2 ){
        return QColor(0,0,255,100);
    }
    else{
        return QColor(255,0,0,100);
    }
}
void SignInTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant pro = editor->property("type");
    if(!pro.isValid()){
        QStyledItemDelegate::setEditorData(editor,index);
        return;
    }
    int type = pro.toInt();
    if(type == Button){
        QPushButton *button = qobject_cast<QPushButton*>(editor);
        int data = index.model()->data(index).toInt();
        button->setStyleSheet(QString("background-color:%1").arg(getColor(data).name()));
    }
    else if(type == CheckBox){
        QCheckBox *checkbox = qobject_cast<QCheckBox*>(editor);
        int data = index.model()->data(index).toInt();
        if(data<=0){
            checkbox->setCheckState(Qt::Unchecked);
        }
        else{
            checkbox->setCheckState(Qt::Checked);
        }
    }
    else if(type == SpinBox){
        QSpinBox *spinbox = qobject_cast<QSpinBox*>(editor);
        int data = index.model()->data(index).toInt();
        spinbox->setValue(data);
    }
    else if(type == ComboBox){
        QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
        for (int i = 0; i < comboBox->count(); ++i) {
            if (comboBox->itemData(i) == index.data()) {
                comboBox->setCurrentIndex(i);
                break;
            }
        }
    }
    else if(type == DateEdit){
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        dateEdit->setDate(index.data().toDate());
    }
}

void SignInTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QVariant pro = editor->property("type");
    if(!pro.isValid()){
        QStyledItemDelegate::setModelData(editor,model,index);
        return;
    }
    int type = pro.toInt();
    if(type == CheckBox){
        QCheckBox *checkbox = qobject_cast<QCheckBox*>(editor);
        if(checkbox->checkState()==Qt::Checked){
            model->setData(index,1);
        }
        else if(checkbox->checkState()==Qt::Unchecked){
            model->setData(index,0);
        }
    }
    else if(type == SpinBox){
        QSpinBox *spinbox = qobject_cast<QSpinBox*>(editor);
        qDebug()<<"spinbox:changed value is "<<spinbox->value();
        model->setData(index,spinbox->value());
    }
    else if(type == ComboBox){
        QComboBox *comboBox = qobject_cast<QComboBox*>(editor);
        model->setData(index,comboBox->currentData());
    }
    else if(type == DateEdit){
        QDateEdit *dateEdit = qobject_cast<QDateEdit*>(editor);
        model->setData(index,dateEdit->date());
    }
}

void SignInTableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void SignInTableDelegate::setColumnType(QList<ColumnType> types)
{
    columntypes = types;
}

void SignInTableDelegate::setComboBoxMap(int column, QMap<QString, QVariant> map)
{
    ComboBoxMaps[column]=map;
}
