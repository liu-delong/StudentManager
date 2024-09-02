#include "stuinfo.h"
#include "ui_stuinfo.h"
#include <QRegExp>
#include <dbmanager.h>
#include <QDebug>
#include <QStringList>
#include <QDateTime>
#include "newstudent.h"
#include <QDialog>
#include <QModelIndexList>
#include "modifystuinfo.h"
StuInfo::StuInfo(QWidget *parent):QWidget(parent),ui(new Ui::stuInfo)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    table = ui->tableView;
    QStringList fieldName;
    for(auto& field:fieldMsg){
        fieldName<<field[0].toString();
    }

    model->setHorizontalHeaderLabels(fieldName);
    sortModel = new QSortFilterProxyModel(this);

    sortModel->setFilterRole(Qt::DisplayRole);
    sortModel->setFilterRegExp(QRegExp("^[1-9]\\d*$"));
    if(ui->checkBox->isChecked()){
            sortModel->setFilterKeyColumn(-1);
    }
    else{
        sortModel->setFilterKeyColumn(4);
    }
    sortModel->setSourceModel(model);
    table->setModel(sortModel);
    table->setSortingEnabled(true);
    delegate = new StuInfoTableDelegate(this);
    table->setItemDelegateForColumn(StuInfo::MODIFY,delegate);
    connect(delegate,&StuInfoTableDelegate::ModifyButttonClickSignal,this,&StuInfo::on_ModifyStuInfoButtonClick);
    loadData();


}

StuInfo::~StuInfo()
{
    delete ui;
}

void StuInfo::loadData()
{
    QString sql = "select *,price/total as unit_price,total-has_attend as remain, concat(name,' ',class) as nameClass  from stu_Info;";
    DbManager::readIns().excuteQuery(sql,[this,sql](QSqlQuery* q){
        if(!q->isActive()){
            qDebug()<<sql<< " failed(StuInfo::loadData)";
            return;
        }
        model->setRowCount(0);
        stu_id2ModelRow.clear();
        name_class2ModelRow.clear();
        while(q->next()){
            model->setRowCount(model->rowCount()+1);
            int row = model->rowCount()-1;
            for(int i=0;i<fieldMsg.count();i++){
                model->setData(model->index(row,i),q->value(fieldMsg[i][1].toString()));
                if(fieldMsg[i][0].toString()=="修改"){
                    table->openPersistentEditor(sortModel->mapFromSource(model->index(row,i)));
                }
            }
            int stu_id = model->index(row,STU_ID).data().toInt();
            QString name = model->index(row,NAME).data().toString();
            QString Class = model->index(row,CLASS).data().toString();
            stu_id2ModelRow[stu_id] = row;
            name_class2ModelRow[name+" "+Class] = row;
        }
    });
}

QStandardItemModel *StuInfo::getStuInfoModel()
{
    return model;
}

int StuInfo::getStuIdColumn()
{
    for(int i=0;i<fieldMsg.count();i++){
        if(fieldMsg[i][1]=="stu_id"){
            return i;
        }
    }
    return -1;
}

int StuInfo::getNameClassColumn()
{
    for(int i=0;i<fieldMsg.count();i++){
        if(fieldMsg[i][1]=="nameClass"){
            return i;
        }
    }
    return -1;
}

int StuInfo::getStuIdByNameClass(QString nameClass)
{
    int row = name_class2ModelRow.value(nameClass,-1);
    if(row==-1) return -1;
    else return model->index(row,STU_ID).data().toInt();
}

QString StuInfo::getNameClassByStuId(int stu_id)
{
    int row = stu_id2ModelRow.value(stu_id,-1);
    if(row == -1) return "";
    QString name = model->index(row,NAME).data().toString();
    QString Class = model->index(row,CLASS).data().toString();
    return name+ " "+ Class;
}


void StuInfo::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked()){
        sortModel->setFilterKeyColumn(-1);
    }
    else{
        sortModel->setFilterKeyColumn(4);
    }
}


void StuInfo::on_pushButton_clicked()
{
    loadData();
}


void StuInfo::on_pushButton_2_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("新增学员");
    QVBoxLayout* dialogLayout = new QVBoxLayout(&dialog);
    dialogLayout->setContentsMargins(0,0,0,0);
    auto window = new NewStudent(&dialog);
    dialogLayout->addWidget(window);
    connect(window,&NewStudent::addNewStudentSignal,this,&StuInfo::on_ensureAddNewStudentButtonClick);
    connect(this,&StuInfo::addNewStudentResultSignal,window,&NewStudent::onAddNewStudentResult);
    window->show();
    dialog.exec();

}

void StuInfo::on_ModifyStuInfoButtonClick(int stu_id)
{
    QModelIndex startIndex = model->index(0,StuInfo::STU_ID);
    int row = model->match(startIndex,Qt::DisplayRole,stu_id,1,Qt::MatchExactly).first().row();
    auto name = model->data(model->index(row,NAME)).toString();
    auto course = model->data(model->index(row,CLASS)).toString();
    auto total = model->data(model->index(row,TOTAL)).toInt();
    auto price = model->data(model->index(row,PRICE)).toDouble();
    auto introducer = model->data(model->index(row,INTRODUCER)).toString();
    auto payTime = model->data(model->index(row,PAYTIME)).toDateTime();
    auto startTime = model->data(model->index(row,BEGINTIME)).toDateTime();
    auto msg = model->data(model->index(row,MSG)).toString();
    QDialog dialog;
    dialog.setLayout(new QVBoxLayout());
    auto widget = new ModifyStuInfo();
    dialog.layout()->addWidget(widget);
    widget->setData(stu_id,name,course,total,price,introducer,payTime,startTime,msg);
    connect(widget,&ModifyStuInfo::modifyStuInfoSignal,this,&StuInfo::on_ModifySutInfoExecButtonClick);
    connect(this,&StuInfo::modifyStudentResultSignal,widget,&ModifyStuInfo::on_modifyStuInfoResult);
    dialog.exec();
}

void StuInfo::on_ModifySutInfoExecButtonClick(int stu_id, QString name, QString course, int total, double price, QString introducer, QDateTime payTime, QDateTime beginTime, QString msg)
{
    QString sql = "call ModifyStuDentAndReturn(%1,'%2','%3',%4,%5,'%6','%7','%8','%9');";
    sql = sql.arg(stu_id);
    sql = sql.arg(name);
    sql = sql.arg(course);
    sql = sql.arg(total);
    sql = sql.arg(price);
    sql = sql.arg(introducer);
    sql = sql.arg(payTime.toString("yyyy-MM-dd"));
    sql = sql.arg(beginTime.toString("yyyy-MM-dd"));
    sql = sql.arg(msg);
    DbManager::writeIns().excuteQuery(sql,[sql,this,stu_id](QSqlQuery* q){
        auto error = q->lastError();
        if(error.type()!=QSqlError::NoError){

            qDebug()<<sql << QString("error:(%1,%2)").arg(error.nativeErrorCode()).arg(error.type())<<error.text();
        }
        else{
            q->next();
            QModelIndex startIndex = model->index(0,StuInfo::STU_ID);
            int row = model->match(startIndex,Qt::DisplayRole,stu_id,1,Qt::MatchExactly).first().row();
            for(int i=0;i<fieldMsg.count();i++){
                model->setData(model->index(row,i),q->value(fieldMsg[i][1].toString()));
            }
            int stu_id = model->index(row,STU_ID).data().toInt();
            QString name = model->index(row,NAME).data().toString();
            QString Class = model->index(row,CLASS).data().toString();
            stu_id2ModelRow[stu_id] = row;
            name_class2ModelRow[name+" "+Class] = row;
            emit StuInfoModified(stu_id);
        }
        emit modifyStudentResultSignal(error.type(),error.nativeErrorCode());
    });

}


void StuInfo::on_ensureAddNewStudentButtonClick(QString name, QString course, int total, double price, QString introducer, QDateTime payTime, QDateTime beginTime,QString msg)
{
    QString sql = "call InsertStuDentAndReturn('%1','%2',%3,%4,'%5','%6','%7','%8');";
    sql = sql.arg(name);
    sql = sql.arg(course);
    sql = sql.arg(total);
    sql = sql.arg(price);
    sql = sql.arg(introducer);
    sql = sql.arg(payTime.toString("yyyy-MM-dd"));
    sql = sql.arg(beginTime.toString("yyyy-MM-dd"));
    sql = sql.arg(msg);
    DbManager::writeIns().excuteQuery(sql,[sql,this](QSqlQuery* q){
        auto error = q->lastError();
        if(error.type()!=QSqlError::NoError){

            qDebug()<<sql << QString("error:(%1,%2)").arg(error.nativeErrorCode()).arg(error.type())<<error.text();
        }
        else{
            q->next();
            model->setRowCount(model->rowCount()+1);
            int row = model->rowCount()-1;
            for(int i=0;i<fieldMsg.count();i++){
                model->setData(model->index(row,i),q->value(fieldMsg[i][1].toString()));
                if(fieldMsg[i][0].toString()=="修改"){
                    table->openPersistentEditor(sortModel->mapFromSource(model->index(row,i)));
                }
            }
            int stu_id = model->index(row,STU_ID).data().toInt();
            QString name = model->index(row,NAME).data().toString();
            QString Class = model->index(row,CLASS).data().toString();
            stu_id2ModelRow[stu_id] = row;
            name_class2ModelRow[name+" "+Class] = row;
            emit newStudentAdded(stu_id);
        }
        emit addNewStudentResultSignal(error.type(),error.nativeErrorCode());
    });
}

void StuInfo::on_signInRecordDeleted(int , int stu_id)
{
    QString sql = "select stu_id,has_attend, total-stu_info.has_attend as remain,stu_info.sign_in_count_a_day from stu_info where stu_id = %1;";
    sql = sql.arg(stu_id);
    DbManager::readIns().excuteQuery(sql,[stu_id,this](QSqlQuery *q){
        q->next();
        if(q->isValid()){
            int has_attend = q->value("has_attend").toInt();
            int remain = q->value("remain").toInt();
            for(int i=0;i<model->rowCount();i++){
                if(model->index(i,STU_ID).data().toInt() == stu_id){
                    model->setData(model->index(i,HAS_ATTEND),has_attend);
                    model->setData(model->index(i,REMAIN),remain);
                    break;
                }
            }
        }
    });
}

void StuInfo::on_stuSignIned(int stu_id, int has_attend, int remain)
{
    for(int i=0;i<model->rowCount();i++){
        if(model->index(i,STU_ID).data().toInt() == stu_id){
            model->setData(model->index(i,HAS_ATTEND),has_attend);
            model->setData(model->index(i,REMAIN),remain);
            break;
        }
    }
}



