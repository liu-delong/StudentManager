#include "datetimemanager.h"
#include <QDate>
#include "timermanager.h"
#include <QDebug>
DateTimeManager::DateTimeManager(QObject *parent)
    : QObject{parent}
{}

int DateTimeManager::getTodayWeekLogical()
{
    return todayWeek;
}

void DateTimeManager::setTodayWeek(int day)
{
    todayWeek=day;
    lastUpdateTodayWeekTime = QDateTime::currentDateTime();
}


const QVector<QTime>& DateTimeManager::getAllClassBeginTimes()
{
    return allClassTimes;
}

int DateTimeManager::getClassTimeIndex(QTime classTime)
{
    return allClassTimes.indexOf(classTime);
}

QTime DateTimeManager::getMostLikelyClassBeginTimes()
{
    auto now = QTime::currentTime();
    return findClosesTime(allClassTimes,now);
}

void DateTimeManager::init()
{
    TimerManager::ins().registerTask(3600,&DateTimeManager::updateTodayWeek);
    updateTodayWeek();
}

void DateTimeManager::updateTodayWeek()
{
    auto now = QDateTime::currentDateTime();
    if(now.date() == lastUpdateTodayWeekTime.date()){
        return;
    }
    else{
        todayWeek = now.date().dayOfWeek();
        lastUpdateTodayWeekTime = now;
    }
}

QTime DateTimeManager::findClosesTime(const QVector<QTime> &times, const QTime &targetTime)
{
    QTime closestTime = times.first();
    int minDiff = qAbs(targetTime.secsTo(closestTime));
    for(const QTime& time:times){
        int diff = qAbs(targetTime.secsTo(time));
        if(diff<minDiff){
            minDiff = diff;
            closestTime = time;
        }
    }
    return closestTime;
}

int DateTimeManager::todayWeek;

QVector<QTime> DateTimeManager::allClassTimes ={QTime::fromString("08:30"),QTime::fromString("10:30"),QTime::fromString("14:30")
                                                 ,QTime::fromString("16:30"),QTime::fromString("17:00"),QTime::fromString("19:15")};

QDateTime DateTimeManager::lastUpdateTodayWeekTime = QDateTime::currentDateTime().addDays(-1);
