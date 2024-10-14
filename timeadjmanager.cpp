#include "timeadjmanager.h"
#include "ui_timeadjmanager.h"
#include <QString>
#include <QDate>
#include "dbmanager.h"
#include <QSqlError>
#include "datetimemanager.h"
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QDebug>
QString buttonDisplay[7]{"星期一","星期二","星期三","星期四","星期五","星期六","星期日"};
TimeAdjManager::TimeAdjManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimeAdjManager)
{
    ui->setupUi(this);
    for(int i=0;i<7;i++){
        ui->comboBox_adj->addItem(buttonDisplay[i],i+1);
    }
    ui->comboBox_adj->addItem("放假",40);
    model = new QStandardItemModel();
    ui->tableView->setModel(model);
    model->setHorizontalHeaderLabels(columnNames);
    ui->dateEdit_adj->setDate(QDate::currentDate());
    ui->dateEdit_delete->setDate(QDate::currentDate());
    ui->comboBox_adj->setCurrentIndex(QDate::currentDate().dayOfWeek()-1);
    ui->pushButton_4->click();
}

TimeAdjManager::~TimeAdjManager()
{
    delete ui;
}

void TimeAdjManager::on_midNight()
{
    ui->dateEdit_adj->setDate(QDate::currentDate());
    ui->dateEdit_delete->setDate(QDate::currentDate());
}

void TimeAdjManager::on_pushButton_2_clicked()
{
    QDate adjDate = ui->dateEdit_adj->date();
    int adjTo = ui->comboBox_adj->currentData().toInt();
    QString sql = "insert into TimeAdj(adj_date, weekday) values ('%1',%2);";
    sql = sql.arg(adjDate.toString("yyyy-MM-dd")).arg(adjTo);
    DbManager::writeIns().excuteQuery(sql,[this,adjDate,adjTo](QSqlQuery *q){
        if(q->lastError().nativeErrorCode()=="1062"){
            ui->label_hint->setText("该日期的调休已经存在");
            return;
        }
        else if(q->lastError().type()!=QSqlError::NoError){
            ui->label_hint->setText(QString("操作错误,错误类型:%1,错误号:%2").arg(q->lastError().type()).arg(q->lastError().nativeErrorCode()));
        }
        else{
            ui->label_hint->setText("成功");
            if(adjDate == QDate::currentDate()){
                DateTimeManager::getInstance().setTodayWeek(adjTo);
            }
            ui->pushButton_4->click();
        }
    });
}


void TimeAdjManager::on_pushButton_3_clicked()
{
    QDate adjDate = ui->dateEdit_delete->date();
    QString sql = " delete ignore from TimeAdj where adj_date = '%1';";
    sql = sql.arg(adjDate.toString("yyyy-MM-dd"));
    DbManager::writeIns().excuteQuery(sql,[this](QSqlQuery *q){
        if(q->lastError().type()!=QSqlError::NoError){
            ui->label_hint_delete->setText(QString("操作错误,错误类型:%1,错误号:%2").arg(q->lastError().type()).arg(q->lastError().nativeErrorCode()));
        }
        else{
            ui->label_hint_delete->setText("删除成功");
            ui->pushButton_4->click();
        }
    });
}


void TimeAdjManager::on_pushButton_4_clicked()
{
    QString sql = "select * from TimeAdj order by adj_date DESC ;";
    DbManager::readIns().excuteQuery(sql,[this](QSqlQuery *q){
        model->setRowCount(0);
        while(q->next()){
            int row = model->rowCount();
            model->setRowCount(row+1);
            QDate date = q->value("adj_date").toDate();
            int weekday = q->value("weekday").toInt();
            model->setData(model->index(row,ADJTIME),date);
            model->setData(model->index(row,ADJTO),weekday);
        }
    });
}


void TimeAdjManager::on_tableView_clicked(const QModelIndex &index)
{
    QDate date = index.data().toDate();
    ui->dateEdit_delete->setDate(date);
}

