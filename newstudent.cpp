#include "newstudent.h"
#include "ui_newstudent.h"
#include <QIntValidator>
#include <QDoubleValidator>
#include <QDebug>
#include <QSqlError>
NewStudent::NewStudent(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewStudent)
{
    ui->setupUi(this);
    ui->lineEdit_price->setValidator(new QDoubleValidator());
    ui->dateTimeEdit_beginTime->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_payTime->setDateTime(QDateTime::currentDateTime());
    ui->label_hint->setVisible(false);
}

NewStudent::~NewStudent()
{
    delete ui;
}


void NewStudent::onAddNewStudentResult(int type, QString errorCode)
{
    if(type!=QSqlError::NoError){
        if(errorCode == "1062"){
            ui->label_hint->setText("新增失败：姓名-课程已存在!");
            ui->label_hint->setVisible(true);
        }
        else{
            ui->label_hint->setText(QString("新增失败：数据库错误：%1,%2!").arg(type).arg(errorCode));
            ui->label_hint->setVisible(true);
        }
    }
    else{
        ui->label_hint->setText("新增成功");
        ui->label_hint->setVisible(true);
    }
}

void NewStudent::on_pushButton_clicked()
{
    auto name = ui->lineEdit_name->text();
    auto course = ui->lineEdit_course->text();
    auto total =ui->spinBox_total->text().toInt();
    auto price = ui->lineEdit_price->text().toDouble();
    auto introducer = ui->lineEdit_introducer->text();
    auto payTime = ui->dateTimeEdit_payTime->dateTime();
    auto beginTime = ui->dateTimeEdit_beginTime->dateTime();
    auto msg = ui->lineEdit_msg->text();
    if(name=="" || course == ""){
        ui->label_hint->setText("新增失败：姓名和课程都不能为空!");
        ui->label_hint->setVisible(true);
        return;
    }
    else{
        emit addNewStudentSignal(name,course,total,price,introducer,payTime,beginTime,msg);
        ui->label_hint->setText("新增中...请稍候");
        ui->label_hint->setVisible(true);
    }



}

