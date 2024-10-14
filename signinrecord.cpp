#include "signinrecord.h"
#include "ui_signinrecord.h"
#include <dbmanager.h>
#include <QString>
#include <QDebug>
#include <QSqlRecord>
#include <QVector>
#include <QDate>
#include <QTimer>
#include <QLineEdit>
#include <QPoint>
#include <QScrollArea>
#include <QCompleter>
#include <QStringList>
SignInRecord::SignInRecord(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignInRecord)
{
    ui->setupUi(this);
    table = ui->tableView;
    model = new QStandardItemModel(this);
    model->setColumnCount(headerNames.count());
    for(int i=0;i<model->columnCount();i++){
        model->setHeaderData(i,Qt::Horizontal,headerNames[i]);
    }
    table->setModel(model);
    ui->dateEdit->setDate(QDate::fromString("2000/1/1"));
    ui->dateEdit_2->setDate(QDate::currentDate());
    delegate = new signInRecordTableDelegate(this);
    table->setItemDelegate(delegate);
    connect(delegate,&signInRecordTableDelegate::deleteEnsureButtonClickedSignal,this,&SignInRecord::on_deleteEnsureButtonClicked);
    getSignInRecord();
    ui->comboBox->setCurrentText("");
}

SignInRecord::~SignInRecord()
{
    delete ui;
}

void SignInRecord::getSignInRecord(int stuId, QDate from, QDate to,int page)
{
    QString sql;
    if(stuId>=0){
        sql = R"(select name, class, sign_in_record.*
                    from sign_in_record
                             left join stu_info on sign_in_Record.stu_id = stu_info.stu_id
                    where sign_in_Record.stu_id = %1
                      and class_date between '%2' and '%3'
                    order by recordId DESC
                    LIMIT 100 offset %4;)";
        sql = sql.arg(stuId);
    }
    else{
        sql = R"(select name, class, sign_in_record.*
                    from sign_in_record
                             left join stu_info on sign_in_Record.stu_id = stu_info.stu_id
                    where class_date between '%1' and '%2'
                    order by recordId DESC
                    LIMIT 100 offset %3;)";
    }
    sql = sql.arg(from.toString("yyyy-MM-dd"));
    sql = sql.arg(to.toString("yyyy-MM-dd"));
    sql = sql.arg((page-1)*100);
    DbManager::readIns().excuteQuery(sql,[&,sql,this](QSqlQuery* q){
        if(!q->isActive()){
            qDebug()<<sql<<"query failed！";
            return;
        }
        model->setRowCount(0);
        auto record = q->record();
        auto fieldCount = record.count();
        QVector<QString> fieldName;
        for(int i=0;i<fieldCount;i++){
            fieldName.push_back(record.fieldName(i));
        }
        while(q->next()){
            QMap<QString, QVariant> row;
            for(int i=0;i<fieldCount;i++){
                row[fieldName[i]]=q->value(i);
            }
            addRow(row);
        }
    });
    return;
}

void SignInRecord::getSignInRecord(QDate from, QDate to, int page)
{
    getSignInRecord(-1,from,to,page);
}

void SignInRecord::getSignInRecord(bool force)
{
    static int lastStuId = -999999;
    static QDate lastFrom = QDate::fromString("1970/1/1");
    static QDate lastTo = QDate::fromString("1970/1/1");
    static int lastPage = 1;

    int stuId = -1;
    auto currentIndex = ui->comboBox->currentIndex();
    if(currentIndex>=0){
        stuId = stuInfoModel->data(stuInfoModel->index(currentIndex,stuInfoStuIdColumn)).toInt();
    }
    auto from = ui->dateEdit->date();
    auto to = ui->dateEdit_2->date();
    auto page = ui->spinBox->value();
    if(force || lastStuId!=stuId || lastFrom !=from || lastTo!=to || lastPage !=page){
        getSignInRecord(stuId,from,to,page);
    }
    lastStuId = stuId;
    lastFrom = from;
    lastTo = to;
    lastPage = page;
}

void SignInRecord::addRow(QMap<QString, QVariant> rowData)
{
    model->setRowCount(model->rowCount()+1);
    int row = model->rowCount()-1;
    for(int i=0;i<headerNames.count();i++){
        if(headerNames[i]=="姓名"){
            model->setData(model->index(row,i),rowData["name"]);
        }
        else if(headerNames[i]=="课程"){
            model->setData(model->index(row,i),rowData["class"]);
        }
        else if(headerNames[i]=="stuId"){
            model->setData(model->index(row,i),rowData["stu_id"]);
        }
        else if(headerNames[i]=="上课日期"){
            model->setData(model->index(row,i),rowData["class_date"]);
        }
        else if(headerNames[i]=="上课时间"){
            model->setData(model->index(row,i),rowData["class_begin_time"]);
        }
        else if(headerNames[i]=="签到时间"){
            model->setData(model->index(row,i),rowData["recordTime"]);
        }
        else if(headerNames[i]=="课时数"){
            model->setData(model->index(row,i),rowData["amount"]);
        }
        else if(headerNames[i]=="备注"){
            model->setData(model->index(row,i),rowData["msg"]);
        }
        else if(headerNames[i]=="recordId"){
            model->setData(model->index(row,i),rowData["recordId"]);
        }
        else if(headerNames[i]=="删除记录"){
            model->setData(model->index(row,i),rowData["recordId"]);
            table->openPersistentEditor(model->index(row,i));
        }
    }
}

void SignInRecord::setStuInfoModel(QStandardItemModel *model, int nameClassColumn, int stuIdColumn)
{
    stuInfoModel = model;
    ui->comboBox->setModel(stuInfoModel);
    ui->comboBox->setModelColumn(nameClassColumn);
    QCompleter* c = new QCompleter;
    c->setModel(stuInfoModel);
    c->setCompletionColumn(nameClassColumn);
    c->setFilterMode(Qt::MatchContains);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    ui->comboBox->lineEdit()->setCompleter(c);
    stuInfoNameClassColumn = nameClassColumn;
    stuInfoStuIdColumn = stuIdColumn;
    ui->comboBox->setCurrentText("gggggg");
}

void SignInRecord::on_midNight()
{
    ui->dateEdit_2->setDate(QDate::currentDate());
}

void SignInRecord::on_pushButton_clicked()
{
    getSignInRecord(true);
}


void SignInRecord::on_comboBox_editTextChanged(const QString &arg1)
{
    if(arg1==""){
        ui->comboBox->setCurrentIndex(-1);
    }
}


void SignInRecord::on_pushButton_2_clicked()
{
    ui->comboBox->setCurrentText("");
}


void SignInRecord::on_dateEdit_userDateChanged(const QDate &)
{
    getSignInRecord();
}


void SignInRecord::on_dateEdit_2_userDateChanged(const QDate &)
{
    getSignInRecord();
}


void SignInRecord::on_spinBox_valueChanged(int )
{
    getSignInRecord();
}

void SignInRecord::on_deleteEnsureButtonClicked(int recordId, int row)
{
    int stu_id =model->index(row,2).data().toInt();
    QString sql = "delete from sign_in_Record where recordId = %1;";
    sql = sql.arg(recordId);
    DbManager::writeIns().excuteQuery(sql,[this,sql,recordId,stu_id](QSqlQuery* q){
        if(!q->isActive()){
            qDebug()<<"删除 recordId:"<<recordId<<"失败";
        }
        else{
            qDebug()<<"删除 recordId:"<<recordId<<"成功";
            getSignInRecord(true);
        }
        emit signInRecordDeleted(recordId,stu_id);
    });
}




void SignInRecord::on_comboBox_currentIndexChanged(int )
{
    getSignInRecord();
}

