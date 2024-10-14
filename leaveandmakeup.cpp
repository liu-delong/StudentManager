#include "leaveandmakeup.h"
#include "ui_leaveandmakeup.h"
#include <QLineEdit>
#include <QCompleter>
#include <QDate>
#include "datetimemanager.h"
#include "mainwindow.h"
#include <QDebug>
#include "dbmanager.h"
#include <QStringList>
#include "leaveandmakeuptabledelegate.h"
#include <QList>
LeaveAndMakeup::LeaveAndMakeup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LeaveAndMakeup)
{
    ui->setupUi(this);
    ui->dateEdit_leave->setDate(QDate::currentDate());
    ui->dateEdit_makeUp->setDate(QDate::currentDate());
    ui->dateEdit_begin->setDate(QDate::currentDate().addMonths(-1));
    ui->dateEdit_end->setDate(QDate::currentDate().addMonths(1));
    auto allClassTime = DateTimeManager::getInstance().getAllClassBeginTimes();
    for(auto classTime:allClassTime){
        ui->comboBox_leave_classTime->addItem(classTime.toString("hh:mm:ss"),classTime);
        ui->comboBox_makeUp_classTime->addItem(classTime.toString("hh:mm:ss"),classTime);
    }
    leave_model = new QStandardItemModel();
    makeUp_model = new QStandardItemModel();

    QStringList labels;
    for(int i=0;i<columnNames.count();i++){
        labels<<columnNames[i];
    }
    leave_model->setHorizontalHeaderLabels(labels);
    makeUp_model->setHorizontalHeaderLabels(labels);
    leave_table = ui->tableView_leave_2;
    makeUp_table = ui->tableView_makeUp;
    leave_table->setModel(leave_model);
    makeUp_table->setModel(makeUp_model);
    LeaveAndMakeUpTableDelegate *leave_delegate = new LeaveAndMakeUpTableDelegate(LEAVE);
    LeaveAndMakeUpTableDelegate *makeUp_delegate = new LeaveAndMakeUpTableDelegate(MAKEUP);
    leave_table->setItemDelegateForColumn(REMOVE,leave_delegate);
    makeUp_table->setItemDelegateForColumn(REMOVE,makeUp_delegate);
    connect(leave_delegate,&LeaveAndMakeUpTableDelegate::removeButtonClicked,this,&LeaveAndMakeup::on_deleteButtonClicked);
    connect(makeUp_delegate,&LeaveAndMakeUpTableDelegate::removeButtonClicked,this,&LeaveAndMakeup::on_deleteButtonClicked);
    loadData();

}

LeaveAndMakeup::~LeaveAndMakeup()
{
    delete ui;
}

void LeaveAndMakeup::setStuInfoModel(QStandardItemModel *model, int nameClassColumn, int stuIdColumn)
{
    m_nameClassColumn = nameClassColumn;
    m_stuIdColumn = stuIdColumn;
    m_stuInfoModel = model;

    QCompleter* completer = new QCompleter();
    completer->setModel(model);
    completer->setCompletionColumn(nameClassColumn);
    completer->setFilterMode(Qt::MatchContains);

    ui->comboBox_leave_name->setModel(model);
    ui->comboBox_leave_name->setModelColumn(nameClassColumn);
    ui->comboBox_leave_name->lineEdit()->setCompleter(completer);

    ui->comboBox_makeUp_name->setModel(model);
    ui->comboBox_makeUp_name->setModelColumn(nameClassColumn);
    ui->comboBox_makeUp_name->lineEdit()->setCompleter(completer);

}

void LeaveAndMakeup::loadData(WhichTable whichTable)
{
    QString sql;
    QDate begin = ui->dateEdit_begin->date();
    QDate end = ui->dateEdit_end->date();
    if(whichTable == LEAVE){
        sql ="select name,class,stu_info.stu_id,day,classbeginTime from leave_Record left join stu_info on leave_Record.stu_id = stu_info.stu_id where day between '%1' and '%2';";
    }
    else if(whichTable == MAKEUP){
        sql ="select name,class,stu_info.stu_id,day,classbeginTime from makeupclass_Record left join stu_info on makeupclass_Record.stu_id = stu_info.stu_id where day between '%1' and '%2';";
    }
    else if(whichTable ==ALL){
        loadData(LEAVE);
        loadData(MAKEUP);
        return;
    }
    sql = sql.arg(begin.toString("yyyy-MM-dd"));
    sql = sql.arg(end.toString("yyyy-MM-dd"));
    DbManager::readIns().excuteQuery(sql,[this,whichTable](QSqlQuery *q){
        QStandardItemModel *model = leave_model;
        QTableView *table = leave_table;
        if(whichTable == LEAVE){
            model = leave_model;
            table = leave_table;
        }
        else if(whichTable == MAKEUP){
            model = makeUp_model;
            table = makeUp_table;
        }
        model->setRowCount(0);
        while(q->next()){
            QString name = q->value("name").toString();
            QString course = q->value("class").toString();
            int stu_id = q->value("stu_id").toInt();
            QDate day = q->value("day").toDate();
            QTime classTime = q->value("classbeginTime").toTime();
            model->setRowCount(model->rowCount()+1);
            int row = model->rowCount()-1;
            model->setData(model->index(row,NAME),name);
            model->setData(model->index(row,COURSE),course);
            model->setData(model->index(row,STU_ID),stu_id);
            model->setData(model->index(row,DATE),day);
            model->setData(model->index(row,TIME),classTime);
            model->setData(model->index(row,REMOVE),stu_id);
            table->openPersistentEditor(model->index(row,REMOVE));
        }
    });

}

void LeaveAndMakeup::on_midNight()
{
    ui->dateEdit_leave->setDate(QDate::currentDate());
    ui->dateEdit_makeUp->setDate(QDate::currentDate());
    ui->dateEdit_begin->setDate(QDate::currentDate().addMonths(-1));
    ui->dateEdit_end->setDate(QDate::currentDate().addMonths(1));
}

void LeaveAndMakeup::on_pushButton_leave_clicked()
{
    int exist = ui->comboBox_leave_name->findText(ui->comboBox_leave_name->currentText());
    if(exist<0){
        showHint(QString("不存在 %1 学员，无法为他/她请假").arg(ui->comboBox_leave_name->currentText()));
        return;
    }
    int currentIndex = ui->comboBox_leave_name->currentIndex();
    int stu_id = m_stuInfoModel->index(currentIndex,m_stuIdColumn).data().toInt();
    QDate leaveDate = ui->dateEdit_leave->date();
    QTime classTime = ui->comboBox_leave_classTime->currentData().toTime();

    QString sql = "insert into leave_Record(stu_id, day, classbeginTime) values (%1,'%2','%3');";
    sql =sql.arg(stu_id);
    sql =sql.arg(leaveDate.toString("yyyy-MM-dd"));
    sql =sql.arg(classTime.toString("hh:mm:ss"));
    DbManager::writeIns().excuteQuery(sql,[this,leaveDate,classTime](QSqlQuery *q){
        if(q->lastError().nativeErrorCode()=="1062"){
            showHint(QString("%1 %2 %3 已有请假记录").arg(ui->comboBox_leave_name->currentText()).arg(leaveDate.toString("yyyy-MM-dd")).arg(classTime.toString("hh:mm")));
        }
        loadData(LEAVE);

    });

}


void LeaveAndMakeup::on_pushButton_makeUp_clicked()
{
    int exist = ui->comboBox_makeUp_name->findText(ui->comboBox_makeUp_name->currentText());
    if(exist<0){
        showHint(QString("不存在 %1 学员，无法为他/她补课").arg(ui->comboBox_makeUp_name->currentText()));
        return;
    }
    int currentIndex = ui->comboBox_makeUp_name->currentIndex();
    int stu_id = m_stuInfoModel->index(currentIndex,m_stuIdColumn).data().toInt();
    QDate makeUpDate = ui->dateEdit_makeUp->date();
    QTime classTime = ui->comboBox_makeUp_classTime->currentData().toTime();

    QString sql = "insert into makeupclass_Record(stu_id, day, classbeginTime) values (%1,'%2','%3');";
    sql =sql.arg(stu_id);
    sql =sql.arg(makeUpDate.toString("yyyy-MM-dd"));
    sql =sql.arg(classTime.toString("hh:mm:ss"));
    DbManager::writeIns().excuteQuery(sql,[this,makeUpDate,classTime](QSqlQuery *q){
        if(q->lastError().nativeErrorCode()=="1062"){
            showHint(QString("%1 %2 %3 已有补课记录").arg(ui->comboBox_makeUp_name->currentText()).arg(makeUpDate.toString("yyyy-MM-dd")).arg(classTime.toString("hh:mm")));
        }
        loadData(MAKEUP);
    });
}

void LeaveAndMakeup::on_deleteButtonClicked(int whichType,int row)
{
    QString sql;
    int stu_id;
    QDate date;
    QTime time;
    QString tableName;
    QStandardItemModel *model = leave_model;
    WhichTable whichTable = LEAVE;
    if(whichType == LEAVE){
        model = leave_model;
        tableName = "leave_record";
        whichTable = LEAVE;
    }
    else if(whichType == MAKEUP){
        model = makeUp_model;
        tableName = "makeupclass_record";
        whichTable = MAKEUP;
    }
    stu_id = model->index(row,STU_ID).data().toInt();
    date = model->index(row,DATE).data().toDate();
    time = model->index(row,TIME).data().toTime();
    sql = "delete ignore from %1 where stu_id = %2 and day = '%3' and classbeginTime = '%4';";
    sql = sql.arg(tableName);
    sql = sql.arg(stu_id);
    sql = sql.arg(date.toString("yyyy-MM-dd"));
    sql = sql.arg(time.toString("hh:mm:ss"));
    DbManager::writeIns().excuteQuery(sql,[this,whichTable](QSqlQuery *q){
        if(q->lastError().type() == QSqlError::NoError){
            loadData(whichTable);
        }
    });
}

void LeaveAndMakeup::on_pushButton_flash_clicked()
{
    loadData();
}

