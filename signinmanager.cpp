#include "signinmanager.h"
#include "ui_signinmanager.h"
#include <QDebug>
#include "dbmanager.h"
#include <QSqlQuery>
#include <QDate>
#include <datetimemanager.h>
#include <QVariant>
#include <QThread>
#include <qsqlquery.h>
#include <QBrush>
#include <QString>
SignInManager::SignInManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignInManager)
{
    ui->setupUi(this);
    table = ui->tableView;
    model = new QStandardItemModel(this);
    delegate = new SignInTableDelegate(this);
    delegate->setColumnType(columnType);
    setHeader();
    setClassBeginTimeComboBoxItems(headerNames.indexOf("课程开始时间")+1);
    table->setModel(model);
    table->setItemDelegate(delegate);
    connect(delegate,SIGNAL(buttonClicked(QPushButton*)),this,SLOT(onSignInButtonClick(QPushButton*)));
    loadAllData();
}


SignInManager::~SignInManager()
{
    delete ui;
}

void SignInManager::setTestData()
{
    clearData();
    QList<QString> names;
    QList<QString> courses;
    for(int i=0;i<100;i++){
        QMap<QString,QVariant> data;
        data["姓名"]=QString("张三")+QString::number(i);
        data["课程"]=QString("课程")+QString::number(i);
        data["学生id"]=i;
        addRow(data);
    }
}

void SignInManager::setHeader()
{
    int colCount = headerNames.count();
    model->setColumnCount(colCount);
    for(int i=0;i<colCount;i++){
        model->setHeaderData(i,Qt::Horizontal,headerNames[i]);
        if(headerNames[i]=="课程开始时间"){
            setClassBeginTimeComboBoxItems(i+1);
        }
    }
}

void SignInManager::setClassBeginTimeComboBoxItems(int column)
{
    QMap<QString,QVariant> classBeginTimeComboBoxItems;
    auto classBeginTime = DateTimeManager::getAllClassBeginTimes();
    for(auto & time:classBeginTime){
        QString text = time.toString("HH:mm:ss");
        classBeginTimeComboBoxItems[text]=time;
    }
    delegate->setComboBoxMap(column,classBeginTimeComboBoxItems);
}

void SignInManager::fixRowData(int row)
{
    for(int i=0;i<headerNames.size();i++){
        if(headerNames[i]=="签到"){
            table->openPersistentEditor(model->index(row,i));
        }
        else if(headerNames[i]=="签到节数"){
            model->setData(model->index(row,i),2);
            //table->openPersistentEditor(model->index(row,i));
        }
        else if(headerNames[i]=="上课日期"){
            model->setData(model->index(row,i),QDate::currentDate());
            //table->openPersistentEditor(model->index(row,i));
        }
        else if(headerNames[i]=="姓名"){
            model->item(row,i)->setEditable(false);
        }
        else if(headerNames[i]=="课程"){
            model->item(row,i)->setEditable(false);
        }
        else if(headerNames[i]=="计划周几上课"){
            model->item(row,i)->setEditable(false);
        }
        else if(headerNames[i]=="总节数"){
            model->item(row,i)->setEditable(false);
        }
        else if(headerNames[i]=="已上节数"){
            model->item(row,i)->setEditable(false);
        }
        else if(headerNames[i]=="剩余节数"){
            model->item(row,i)->setEditable(false);
        }
        else if(headerNames[i]=="学生id"){
           model->item(row,i)->setEditable(false);
        }

        else if(headerNames[i]=="课程开始时间"){
            //model->setData(model->index(row,i),DateTimeManager::getMostLikelyClassBeginTimes());
            table->openPersistentEditor(model->index(row,i));
        }
    }
}

void SignInManager::on_signInRecordDeleted(int , int stu_id)
{
    QString sql = "select stu_id,has_attend, total-stu_info.has_attend as remain,stu_info.sign_in_count_a_day from stu_info where stu_id = %1;";
    sql = sql.arg(stu_id);
    DbManager::readIns().excuteQuery(sql,[stu_id,this](QSqlQuery *q){
        q->next();
        if(q->isValid()){
            int has_attend = q->value("has_attend").toInt();
            int remain = q->value("remain").toInt();
            int signInADay = q->value("sign_in_count_a_day").toInt();
            for(int i=0;i<model->rowCount();i++){
                if(model->index(i,11).data().toInt() == stu_id){
                    model->setData(model->index(i,9),has_attend);
                    model->setData(model->index(i,10),remain);
                    model->setData(model->index(i,3),signInADay);
                    break;
                }
            }
        }
    });
}

void SignInManager::on_classScheduleChanged()
{
    loadAllData();
}


void SignInManager::loadAllData(int weekday,QTime now)
{
    DbManager::readIns().excuteQuery(QString("call getclassschedule(%1,'%2');").arg(weekday).arg(now.toString("HH:mm:ss")),[&,this](QSqlQuery* q){
        allData.clear();
        while(q->next()){
            QMap<QString,QVariant> row;
            row["姓名"]=q->value(0);
            row["课程"]=q->value(1);
            row["课程开始时间"]=q->value("classBeginTime");
            row["总节数"]=q->value("total");
            row["已上节数"]=q->value("has_attend");
            row["剩余节数"]=q->value("remain");
            row["学生id"]=q->value("stu_id");
            row["计划周几上课"]=q->value("weekday");
            row["备注"]=q->value("message");
            if(q->value("last_sign_in_date").toDate()==QDate::currentDate()){
                row["签到"] = q->value("sign_in_count_a_day");
            }
            else{
                row["签到"] = 0;
            }
            allData.append(std::move(row));
        }
        clearData();
        for(auto& row:allData){
            addRow(row);
        }
    });
}

void SignInManager::loadAllData()
{
    loadAllData(DateTimeManager::getTodayWeekLogical(),QTime::currentTime());
}


void SignInManager::addRow(QMap<QString, QVariant> data)
{
    model->setRowCount(model->rowCount()+1);
    int row = model->rowCount()-1;
    for(int i=0;i<headerNames.count();i++){
        QString field = headerNames[i];
        QVariant value = data.value(field,"");
        model->setData(model->index(row,i),value);
    }
    fixRowData(row);

}

void SignInManager::clearData()
{
    model->clear();
    model->setRowCount(0);
    setHeader();
}


void SignInManager::signIn(int row)
{
    int stuId = (model->data(model->index(row,headerNames.indexOf("学生id")))).toInt();
    QDate classDate = (model->data(model->index(row,headerNames.indexOf("上课日期")))).toDate();
    QTime classTime = (model->data(model->index(row,headerNames.indexOf("课程开始时间")))).toTime();
    int amount = (model->data(model->index(row,headerNames.indexOf("签到节数")))).toInt();
    QString msg = (model->data(model->index(row,headerNames.indexOf("备注")))).toString();
    QString sql = "call signIn(%1,'%2','%3',%4,'%5');";
    sql=sql.arg(stuId);
    sql=sql.arg(classDate.toString("yyyy-MM-dd"));
    sql=sql.arg(classTime.toString("hh:mm:ss"));
    sql=sql.arg(amount);
    sql=sql.arg(msg);
    DbManager::writeIns().excuteQuery(sql,[row,this](QSqlQuery* query){
        if(query->next()){
            qDebug()<<" has_next";
            int stu_id = query->value("stu_id").toInt();
            QDate last_sign_in_date = query->value("last_sign_in_date").toDate();
            int sign_in_count_a_day = query->value("sign_in_count_a_day").toInt();
            int total = query->value("total").toInt();
            int has_attend = query->value("has_attend").toInt();
            int remain = query->value("remain").toInt();
            onSignInFinished(0,last_sign_in_date,sign_in_count_a_day,total,has_attend,remain,stu_id,row);
            emit stuSignIned(stu_id,has_attend,remain);
        }
    });
}

void SignInManager::onSignInButtonClick(QPushButton *button)
{
    QVariant pro = button->property("row");
    if(!pro.isValid()){
        return;
    }
    int row = pro.toInt();
    signIn(row);
}


void SignInManager::onSignInFinished(int status, QDate , int sign_in_count_a_day, int total, int has_attend, int remain, int stuId,int row)
{
    if(status !=0){
        qDebug()<<"stu_id:"<<stuId<<"签到失败";
        return;
    }
    int signInButtionColumn = -1;
    int totalColumn = -1;
    int hasAttendColumn = -1;
    int remainColumn = -1;
    for(int i=0;i<model->columnCount();i++){
        QString columnName = model->horizontalHeaderItem(i)->text();
        if(columnName== "签到"){
            signInButtionColumn = i;
        }
        else if(columnName == "总节数"){
            totalColumn = i;
        }
        else if(columnName == "已上节数"){
            hasAttendColumn = i;
        }
        else if(columnName == "剩余节数"){
            remainColumn = i;
        }
    }
    if(signInButtionColumn == -1){
        qDebug()<<"onSignInFinished：找不到签到列";
    }
    else{
        model->setData(model->index(row,signInButtionColumn),sign_in_count_a_day);
    }
    if(  totalColumn== -1){
        qDebug()<<"onSignInFinished：找不到总节数列";
    }
    else{
        model->setData(model->index(row,totalColumn),total);
    }
    if(hasAttendColumn == -1){
        qDebug()<<"onSignInFinished：找不到已上节数列";
    }
    else{
        model->setData(model->index(row,hasAttendColumn),has_attend);
    }
    if(remainColumn == -1){
        qDebug()<<"onSignInFinished：找不到剩余节数列";
    }
    else{
        model->setData(model->index(row,remainColumn),remain);
    }
}


void SignInManager::on_pushButton_flash_clicked()
{
    loadAllData();
}

