#include "report.h"
#include "ui_report.h"
#include <QDate>
#include <QString>
#include <QStandardItemModel>
#include <QDateEdit>
#include "dbmanager.h"
#include <QVector>
#include <QSortFilterProxyModel>
Report::Report(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Report)
{
    ui->setupUi(this);
    for(int i=0;i<12;i++){
        QString buttonName = QString("pushButton_month_%1").arg(i+1);
        auto button = findChild<QPushButton*>(buttonName);
        connect(button,&QPushButton::clicked,this,[i,this](){
            int year = ui->dateEdit_begin->date().year();
            ui->dateEdit_begin->setDate(QDate(year,i+1,1));
            ui->dateEdit_end->setDate(QDate(year,i+1,QDate(year, i+1, 1).daysInMonth()));
            ui->pushButton_flash_stu_view->click();
        });
    }
    ui->pushButton_this_year->click();
    monthViewModel = new QStandardItemModel();
    stuViewModel = new QStandardItemModel();
    monthViewModel->setColumnCount(monthViewColumnNames.count());
    for(int i=0;i<monthViewColumnNames.count();i++){
        monthViewModel->setHorizontalHeaderItem(i,new QStandardItem(monthViewColumnNames[i]));
    }
    stuViewModel->setColumnCount(stuViewColumnNames.count());
    for(int i=0;i<stuViewColumnNames.count();i++){
        stuViewModel->setHorizontalHeaderItem(i,new QStandardItem(stuViewColumnNames[i]));
    }
    stuViewSortModel = new QSortFilterProxyModel(this);
    stuViewSortModel->setSourceModel(stuViewModel);

    monthViewModel->setRowCount(12);
    for(int i=0;i<12;i++){
        monthViewModel->setData(monthViewModel->index(i,MONTH),QString("%1月").arg(i+1));
    }
    ui->tableView_month_view->setModel(monthViewModel);
    ui->tableView_stu_view->setModel(stuViewSortModel);
    ui->tableView_stu_view->setSortingEnabled(true);
    ui->dateEdit_year->setDate(QDate::currentDate());
    loadMonthViewData();
    loadStuViewData();
    connect(ui->pushButton_flash_month_view,&QPushButton::clicked,this,&Report::loadMonthViewData);
    connect(ui->pushButton_flash_stu_view,&QPushButton::clicked,this,&Report::loadStuViewData);
}

Report::~Report()
{
    delete ui;
}

void Report::loadMonthViewData()
{
    int year = ui->dateEdit_year->date().year();
    QString sql = "select MONTH(sign_in_Record.class_date) as month ,sum(amount*(price/total)) as money "
                  "from sign_in_Record left join stu_info si on sign_in_Record.stu_id = si.stu_id "
                  "where year(class_date) = %1 group by MONTH(class_date);";
    sql = sql.arg(year);
    DbManager::readIns().excuteQuery(sql,[this](QSqlQuery* q){
        QVector<double> monthMoney(12,0);
        while(q->next()){
            int month = q->value("month").toInt()-1;
            monthMoney[month] = q->value("money").toDouble();
        }
        for(int i=0;i<12;i++){
            monthViewModel->setData(monthViewModel->index(i,MONTH_VIEW_TOTAL),monthMoney[i]);
        }
    });

}

void Report::loadStuViewData()
{
    auto begin = ui->dateEdit_begin->date();
    auto end = ui->dateEdit_end->date();
    QString sql = "select stu_info.stu_id,name,class,price/stu_info.total as aprice,IFNULL(classAmount,0) as classAmount,"
                  "price/stu_info.total*IFNULL(classAmount,0) as totalMoney from stu_info left join ("
                    "select stu_id,sum(amount) as classAmount from sign_in_Record where class_date between '%1' and '%2' group by stu_id"
                  ") as subTable on stu_info.stu_id = subTable.stu_id;";
    sql =sql.arg(begin.toString("yyyy-MM-dd"));
    sql = sql.arg(end.toString("yyyy-MM-dd"));
    DbManager::readIns().excuteQuery(sql,[this](QSqlQuery* q){
        stuViewModel->setRowCount(0);
        while(q->next()){
            int row= stuViewModel->rowCount();
            stuViewModel->setRowCount(row+1);
            stuViewModel->setData(stuViewModel->index(row,STU_ID),q->value("stu_id"));
            stuViewModel->setData(stuViewModel->index(row,NAME),q->value("name"));
            stuViewModel->setData(stuViewModel->index(row,COURSE),q->value("class"));
            stuViewModel->setData(stuViewModel->index(row,PRICE),q->value("aprice"));
            stuViewModel->setData(stuViewModel->index(row,AMOUNT),q->value("classAmount"));
            stuViewModel->setData(stuViewModel->index(row,STU_VIEW_TOTAL),q->value("totalMoney"));
        }
    });
}

void Report::on_pushButton_last_year_clicked()
{
    ui->dateEdit_begin->setDate(QDate(QDate::currentDate().year()-1,1,1));
    ui->dateEdit_end->setDate(QDate(QDate::currentDate().year()-1,12,31));
    ui->pushButton_flash_stu_view->click();
}


void Report::on_pushButton_this_year_clicked()
{
    ui->dateEdit_begin->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEdit_end->setDate(QDate(QDate::currentDate().year(),12,31));
    ui->pushButton_flash_stu_view->click();
}


void Report::on_pushButton_flash_month_view_clicked()
{
    loadMonthViewData();
}


void Report::on_pushButton_flash_stu_view_clicked()
{
    loadStuViewData();
}


void Report::on_dateEdit_year_dateChanged(const QDate &date)
{
    loadMonthViewData();
}

