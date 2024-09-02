#include "report.h"
#include "ui_report.h"
#include <QDate>
#include <QString>
#include <QStandardItemModel>
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
    ui->tableView_month_view->setModel(monthViewModel);
    ui->tableView_stu_view->setModel(stuViewModel);

}

Report::~Report()
{
    delete ui;
}

void Report::on_pushButton_last_year_clicked()
{
    ui->dateEdit_begin->setDate(QDate(QDate::currentDate().year()-1,1,1));
    ui->dateEdit_end->setDate(QDate(QDate::currentDate().year()-1,12,31));
}


void Report::on_pushButton_this_year_clicked()
{
    ui->dateEdit_begin->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEdit_end->setDate(QDate(QDate::currentDate().year(),12,31));
}

