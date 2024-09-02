#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include "signinrecord.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ui->textBrowser->setWordWrapMode(QTextOption::NoWrap);
    // ui->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    signInManager = ui->page;
    signInRecord = ui->page_2;
    stuInfo = ui->page_3;
    classSchedule = ui->page_4;
    leaveAndMakeup = ui->page_5;

    connect(signInRecord,&SignInRecord::signInRecordDeleted,signInManager,&SignInManager::on_signInRecordDeleted);
    connect(signInRecord,&SignInRecord::signInRecordDeleted,stuInfo,&StuInfo::on_signInRecordDeleted);
    connect(signInManager,&SignInManager::stuSignIned,stuInfo,&StuInfo::on_stuSignIned);
    connect(classSchedule,&ClassSchedule::classScheduleChange,signInManager,&SignInManager::on_classScheduleChanged);
    connect(stuInfo,&StuInfo::StuInfoModified,signInManager,QOverload<>::of(&SignInManager::loadAllData));
    connect(stuInfo,&StuInfo::newStudentAdded,signInManager,QOverload<>::of(&SignInManager::loadAllData));
}

MainWindow &MainWindow::getInstance()
{
    static MainWindow mainWindow;
    return mainWindow;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showHint(QString msg)
{
    ui->textBrowser->append(msg);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    SignInRecord *widget = qobject_cast<SignInRecord *>( ui->stackedWidget->widget(1));
    if(widget){
        widget->getSignInRecord(true);
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    //StuInfo *widget = qobject_cast<StuInfo *>( ui->stackedWidget->widget(1));

}


void MainWindow::on_pushButton_classSchedule_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}


void showHint(QString msg)
{
    MainWindow::getInstance().showHint(msg);
}
