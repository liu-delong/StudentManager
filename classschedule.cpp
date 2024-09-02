#include "classschedule.h"
#include "ui_classschedule.h"
#include "datetimemanager.h"
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <dbmanager.h>
#include <mainwindow.h>
#include <QStandardItem>
#include <QStandardItemModel>
#include "mylistview.h"
QString weekname[7] ={"一","二","三","四","五","六","日"};
ClassSchedule::ClassSchedule(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClassSchedule)
{
    ui->setupUi(this);
    ui->listView_stu->setDragEnabled(true);                  // 启用拖拽
    ui->listView_stu->setDefaultDropAction(Qt::MoveAction);  // 设置默认的拖放操作为移动

    trash_model = new QStandardItemModel();
    ui->listView_trash->setModel(trash_model);


    ui->listView_trash->setDragEnabled(true);                  // 启用拖拽
    ui->listView_trash->setAcceptDrops(true);                  // 接受放置
    ui->listView_trash->setDropIndicatorShown(true);           // 显示放置指示
    ui->listView_trash->setDefaultDropAction(Qt::MoveAction);  // 设置默认的拖放操作为移动
    ui->listView_trash->setEnabled(false);
    classTable.resize(7);
    classTableModel.resize(7);
    auto allClassTimes = DateTimeManager::getAllClassBeginTimes();\
    qDebug()<<allClassTimes;
    int classTimeCount = allClassTimes.count();
    for(int i=0;i<7;i++){
        classTable[i].resize(classTimeCount);
        classTableModel[i].resize(classTimeCount);
    }
    QGridLayout* layout = (QGridLayout*)ui->widget_classTable->layout();
    for(int i=0;i<7;i++){
        layout->addWidget(new QLabel(QString("星期%1").arg(weekname[i])),0,i+1);
    }
    for(int i=0;i<classTimeCount;i++){
        layout->addWidget(new QLabel(allClassTimes[i].toString("hh:mm")),i+1,0);
    }
    for(int i=0;i<7;i++){
        for(int j=0;j<classTimeCount;j++){
            auto listView = new MyListView();
            auto model = new QStandardItemModel();
            model->setProperty("day",i+1);
            model->setProperty("classTime",allClassTimes[j]);
            listView->setDropIndicatorShown(true);           // 显示放置指示
            listView->setDefaultDropAction(Qt::MoveAction);  // 设置默认的拖放操作为移动
            listView->setModel(model);
            listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            QGridLayout* layout = (QGridLayout*)ui->widget_classTable->layout();
            layout->addWidget(listView,j+1,i+1);
            classTable[i][j] = listView;
            classTableModel[i][j] = model;

        }
    }
    changeToNormalMode();
    loadData();
}

ClassSchedule::~ClassSchedule()
{
    delete ui;
}

void ClassSchedule::setModel(QStandardItemModel *model, int column)
{
    ui->listView_stu->setModel(model);
    ui->listView_stu->setModelColumn(column);
}

void ClassSchedule::loadData()
{
    bool showHasFinished = ui->checkBox->isChecked();
    QString sql = "select si.stu_id,weekday,classBeginTime,concat(name,' ',class) as nameClass from class_time left join stu_info si on class_time.stu_id = si.stu_id";
    if(!showHasFinished){
        sql += " where priority>0;";
    }
    else{
        sql += ";";
    }
    DbManager::readIns().excuteQuery(sql,[sql,this](QSqlQuery *q){
        if(!q->isActive()){
            qDebug()<<"ClassSchedule::loadData() 读取课程表失败";
            return;
        }
        for(int i=0;i<classTableModel.count();i++){
            auto &classTableADay = classTableModel[i];
            for(int j=0;j<classTableADay.count();j++){
                classTableModel[i][j]->clear();
            }
        }
        while(q->next()){
            int weekday = q->value("weekday").toInt();
            QTime classTime = q->value("classBeginTime").toTime();
            QString nameClass = q->value("nameClass").toString();
            int classTimeIndex = DateTimeManager::getClassTimeIndex(classTime);
            if(classTimeIndex<0 || classTimeIndex >=classTableModel[0].count()) continue;
            if(weekday<1 || weekday>7) continue;
            classTableModel[weekday-1][classTimeIndex]->appendRow(new QStandardItem(nameClass));

        }

    });


}


void ClassSchedule::on_pushButton_edit_clicked()
{
    QPushButton* bt = (QPushButton*)sender();
    QString text = bt->text();
    if(text == "编辑"){
        changeToEditMode();
    }
    else if(text == "确认"){
        changeToSubmitMode();
        submitChange();
    }
}

void ClassSchedule::on_rowsInserted(const QModelIndex &topLeft, const QModelIndex &, const QVector<int> &)
{
    QStandardItemModel* senderModel = (QStandardItemModel*) sender();
    int weekday = senderModel->property("day").toInt();
    QTime classTime = senderModel->property("classTime").toTime();
    QString nameClass = senderModel->data(topLeft).toString();
    auto stuInfo = MainWindow::getInstance().stuInfo;
    int stu_id = stuInfo->getStuIdByNameClass(nameClass);
    Command_metaData metaData = {stu_id,weekday,classTime};
    if(commands.contains(metaData)){
        commands[metaData]+=1;
    }
    else{
        commands[metaData] = 1;
    }

}

void ClassSchedule::on_rowsAboutToBeRemoved(const QModelIndex &, int first, int )
{
    QStandardItemModel* senderModel = (QStandardItemModel*) sender();
    int weekday = senderModel->property("day").toInt();
    QTime classTime = senderModel->property("classTime").toTime();
    QString nameClass = senderModel->index(first,0).data().toString();
    auto stuInfo = MainWindow::getInstance().stuInfo;
    int stu_id = stuInfo->getStuIdByNameClass(nameClass);
    Command_metaData metaData = {stu_id,weekday,classTime};
    if(commands.contains(metaData)){
        commands[metaData]-=1;
    }
    else{
        commands[metaData] =-1;
    }
}

void ClassSchedule::changeToEditMode()
{
    commands.clear();
    for(int i=0;i<7;i++){
        auto& tableADay = classTable[i];
        for(int j=0;j<tableADay.count();j++){
            auto tableATime = tableADay[j];
            tableATime->setDragEnabled(true);                  // 启用拖拽
            tableATime->setAcceptDrops(true);                  // 接受放置
            auto m = classTableModel[i][j];
            connect(m,&QAbstractItemModel::dataChanged,this,&ClassSchedule::on_rowsInserted);
            connect(m,&QAbstractItemModel::rowsAboutToBeRemoved,this,&ClassSchedule::on_rowsAboutToBeRemoved);
        }
    }
    ui->listView_trash->setEnabled(true);
    ui->pushButton_flash->setEnabled(false);
    ui->pushButton_edit->setText("确认");
    ui->pushButton_edit->setEnabled(true);
}

void ClassSchedule::changeToSubmitMode()
{
    for(int i=0;i<7;i++){
        auto& tableADay = classTable[i];
        for(int j=0;j<tableADay.count();j++){
            auto tableATime = tableADay[j];
            tableATime->setDragEnabled(false);                  // 禁止拖拽
            tableATime->setAcceptDrops(false);                  // 不接受放置
            auto m = classTableModel[i][j];
            disconnect(m,&QAbstractItemModel::dataChanged,this,&ClassSchedule::on_rowsInserted);
            disconnect(m,&QAbstractItemModel::rowsAboutToBeRemoved,this,&ClassSchedule::on_rowsAboutToBeRemoved);
        }
    }
    ui->listView_trash->setEnabled(false);
    ui->pushButton_flash->setEnabled(false);
    ui->pushButton_edit->setText("修改提交中");
    ui->pushButton_edit->setEnabled(false);
}

void ClassSchedule::changeToNormalMode()
{
    for(int i=0;i<7;i++){
        auto& tableADay = classTable[i];
        for(int j=0;j<tableADay.count();j++){
            auto tableATime = tableADay[j];
            tableATime->setDragEnabled(false);                  // 启用拖拽
            tableATime->setAcceptDrops(false);                  // 接受放置
            auto m = classTableModel[i][j];
            disconnect(m,&QAbstractItemModel::dataChanged,this,&ClassSchedule::on_rowsInserted);
            disconnect(m,&QAbstractItemModel::rowsAboutToBeRemoved,this,&ClassSchedule::on_rowsAboutToBeRemoved);
        }
    }
    ui->listView_trash->setEnabled(false);
    trash_model->clear();
    ui->pushButton_flash->setEnabled(true);
    ui->pushButton_edit->setText("编辑");
    ui->pushButton_edit->setEnabled(true);
}

void ClassSchedule::submitChange()
{
    qDebug()<<commands;
    QStringList sqls;
    for(auto it =commands.constBegin();it!=commands.constEnd();++it){
        auto key = it.key();
        auto value = it.value();
        auto stu_id = key.stu_id;
        auto weekday = key.weekday;
        auto classTime = key.classTime;
        if(value>0){
            QString sql = "insert ignore into class_time(stu_id,weekday,classBeginTime) values(%1,%2,'%3');";
            sql =sql.arg(stu_id);
            sql =sql.arg(weekday);
            sql =sql.arg(classTime.toString("hh:mm:ss"));
            sqls<<sql;
        }
        else if(value<0){
            QString sql = "delete ignore from class_time where stu_id = %1 and weekday = %2 and classBeginTime = '%3';";
            sql =sql.arg(stu_id);
            sql =sql.arg(weekday);
            sql =sql.arg(classTime.toString("hh:mm:ss"));
            sqls<<sql;
        }
        else if(value == 0){}
    }
    DbManager::writeIns().excuteQuery(sqls,[this](QVector<QSqlQuery*> qs){
        emit classScheduleChange();
        changeToNormalMode();
    });

}


void ClassSchedule::on_pushButton_flash_clicked()
{
    loadData();
}

