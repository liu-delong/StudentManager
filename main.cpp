#include "mainwindow.h"

#include <QApplication>
#include <functional>
#include <QSqlQuery>
#include <QDebug>
#include <datetimemanager.h>
#include <QWidget>
#include <QBasicTimer>
Q_DECLARE_METATYPE(std::function<void(QSqlQuery*)>)
Q_DECLARE_METATYPE(std::function<void(QVector<QSqlQuery*>)>)
Q_DECLARE_METATYPE(QVector<QSqlQuery*>)
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<std::function<void(QSqlQuery*)>>("std::function<void(QSqlQuery*)>");
    qRegisterMetaType<std::function<void(QVector<QSqlQuery*>)>>("std::function<void(QVector<QSqlQuery*>)>");
    qRegisterMetaType<QVector<QSqlQuery*>>("QVector<QSqlQuery*>");
    DateTimeManager::getInstance().init();
    MainWindow::getInstance().show();
    auto stuInfo = MainWindow::getInstance().stuInfo;
    auto signInManager = MainWindow::getInstance().signInManager;
    auto signInRecord = MainWindow::getInstance().signInRecord;
    auto classSchedule = MainWindow::getInstance().classSchedule;
    auto leaveAndMakeUp = MainWindow::getInstance().leaveAndMakeup;

    signInRecord->setStuInfoModel(stuInfo->getStuInfoModel(),stuInfo->getNameClassColumn(),stuInfo->getStuIdColumn());
    classSchedule->setModel(stuInfo->getStuInfoModel(),stuInfo->getNameClassColumn());
    leaveAndMakeUp->setStuInfoModel(stuInfo->getStuInfoModel(),stuInfo->getNameClassColumn(),stuInfo->getStuIdColumn());
    int ret = a.exec();
    return ret;
}
