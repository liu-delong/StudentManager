#include "modifystuinfo.h"
#include "ui_modifystuinfo.h"
#include <QSqlError>

ModifyStuInfo::ModifyStuInfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModifyStuInfo)
{
    ui->setupUi(this);
    ui->label_hint->setVisible(false);
}

void ModifyStuInfo::setData(int stu_id,QString name, QString course, int total, double price, QString introducer, QDateTime payTime, QDateTime beginTime, QString msg)
{
    m_stu_id = stu_id;
    ui->label_name->setText(QString("姓名(")+name+")->");
    ui->label_course->setText(QString("课程(")+course+")->");
    ui->label_total->setText(QString("总课时(")+QString::number(total)+")->");
    ui->label_price->setText(QString("学费(")+QString::number(price)+")->");
    ui->label_introducer->setText(QString("介绍人(")+introducer+")->");
    ui->label_payTime->setText(QString("缴费时间(")+payTime.toString("yyyy-MM-dd hh:mm:ss")+")->");
    ui->label_beginTime->setText(QString("第一次上课时间(")+beginTime.toString("yyyy-MM-dd hh:mm:ss")+")->");
    ui->label_msg->setText(QString("备注(")+msg+")->");
    ui->lineEdit_name->setText(name);
    ui->lineEdit_course->setText(course);
    ui->lineEdit_total->setText(QString::number(total));
    ui->lineEdit_price->setText(QString::number(price));
    ui->lineEdit_introducer->setText(introducer);
    ui->dateTimeEdit_payTime->setDateTime(payTime);
    ui->dateTimeEdit_beginTime->setDateTime(beginTime);
}

void ModifyStuInfo::setModifyStuInfoResult(int type, QString errorCode)
{
    if(type!=QSqlError::NoError){
        if(errorCode == "1062"){
            ui->label_hint->setText("修改失败：姓名-课程已存在!");
            ui->label_hint->setVisible(true);
        }
        else{
            ui->label_hint->setText(QString("修改失败：数据库错误：%1,%2!").arg(type).arg(errorCode));
            ui->label_hint->setVisible(true);
        }
    }
    else{
        ui->label_hint->setText("修改成功");
        auto name = ui->lineEdit_name->text();
        auto course = ui->lineEdit_course->text();
        auto total = ui->lineEdit_total->text().toInt();
        auto price = ui->lineEdit_price->text().toDouble();
        auto introducer = ui->lineEdit_introducer->text();
        auto payTime = ui->dateTimeEdit_payTime->dateTime();
        auto beginTime = ui->dateTimeEdit_beginTime->dateTime();
        auto msg = ui->lineEdit_msg->text();
        setData(m_stu_id,name,course,total,price,introducer,payTime,beginTime,msg);
    }
    ui->label_hint->setVisible(true);
    ui->lineEdit_name->setEnabled(true);
    ui->lineEdit_course->setEnabled(true);
    ui->lineEdit_price->setEnabled(true);
    ui->lineEdit_total->setEnabled(true);
    ui->lineEdit_introducer->setEnabled(true);
    ui->lineEdit_msg->setEnabled(true);
    ui->dateTimeEdit_beginTime->setEnabled(true);
    ui->dateTimeEdit_payTime->setEnabled(true);
    ui->pushButton->setEnabled(true);
}

ModifyStuInfo::~ModifyStuInfo()
{
    delete ui;
}

void ModifyStuInfo::on_modifyStuInfoResult(int type, QString errorCode)
{
    setModifyStuInfoResult(type,errorCode);
}

void ModifyStuInfo::on_pushButton_clicked()
{
    QPushButton* bt = (QPushButton*)sender();
    bt->setEnabled(false);
    ui->lineEdit_name->setEnabled(false);
    ui->lineEdit_course->setEnabled(false);
    ui->lineEdit_price->setEnabled(false);
    ui->lineEdit_total->setEnabled(false);
    ui->lineEdit_introducer->setEnabled(false);
    ui->lineEdit_msg->setEnabled(false);
    ui->dateTimeEdit_beginTime->setEnabled(false);
    ui->dateTimeEdit_payTime->setEnabled(false);
    auto name = ui->lineEdit_name->text();
    auto course = ui->lineEdit_course->text();
    auto total = ui->lineEdit_total->text().toInt();
    auto price = ui->lineEdit_price->text().toDouble();
    auto introducer = ui->lineEdit_introducer->text();
    auto payTime = ui->dateTimeEdit_payTime->dateTime();
    auto beginTime = ui->dateTimeEdit_beginTime->dateTime();
    auto msg = ui->lineEdit_msg->text();
    if(name=="" || course == ""){
        ui->label_hint->setText("修改失败：姓名和课程都不能为空!");
        ui->label_hint->setVisible(true);
        bt->setEnabled(true);
        return;
    }
    emit modifyStuInfoSignal(m_stu_id,name,course,total,price,introducer,payTime,beginTime,msg);
    ui->label_hint->setText("修改中...请稍后");
    ui->label_hint->setVisible(true);
}

