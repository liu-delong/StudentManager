#include "datetimemanager.h"
#include <QDate>
#include <QDebug>
#include "dbmanager.h"
DateTimeManager::DateTimeManager(QObject *parent)
    : QObject{parent}
{}

DateTimeManager &DateTimeManager::getInstance()
{
    static DateTimeManager instance;
    return instance;
}

int DateTimeManager::getTodayWeekLogical()
{
    return todayWeek;
}

void DateTimeManager::setTodayWeek(int day)
{
    todayWeek=day;
}


const QVector<QTime>& DateTimeManager::getAllClassBeginTimes()
{
    return allClassTimes;
}

int DateTimeManager::getClassTimeIndex(QTime classTime)
{
    return allClassTimes.indexOf(classTime);
}


void DateTimeManager::init()
{
    updateTodayWeek();
}

void DateTimeManager::updateTodayWeek()
{
    QString sql  = "select * from TimeAdj where adj_date = current_date();";
    DbManager::readIns().excuteQuery(sql,[this](QSqlQuery *q){
        if(q->next()){
            int day = q->value("weekday").toInt();
            setTodayWeek(day);
        }
        else{
            setTodayWeek(QDate::currentDate().dayOfWeek());
        }
        auto &ins = DateTimeManager::getInstance();
        emit ins.todayWeekchange(todayWeek);
    });

}

void DateTimeManager::on_MidNight()
{
    updateTodayWeek();
}

void DateTimeManager::sendSignalEveryMidnight()
{
    QTime currentTime = QTime::currentTime();
    QTime midnight(0, 0, 1); // 表示0点

    // 计算当前时间到下一个0点的时间差（以毫秒为单位）
    int msecsToMidnight = currentTime.msecsTo(midnight);
    if (msecsToMidnight <= 0) {
        // 如果已经过了0点，那么计算到下一个0点的时间（加一天）
        msecsToMidnight += 24 * 60 * 60 * 1000; // 一天的毫秒数
    }
    qDebug()<<msecsToMidnight/1000/60;
    firstTimer = new QTimer(this);

    // 创建一个定时器，等待到0点
    firstTimer->singleShot(2000, [this]() {
        qDebug()<<"hello";
        emit midNightArrived();
            // 重新设置定时器，每天0点触发
        dailyTimer = new QTimer(this);
        connect(dailyTimer, &QTimer::timeout, this,&DateTimeManager::midNightArrived);
        dailyTimer->start(24 * 60 * 60 * 1000);
    });
}

