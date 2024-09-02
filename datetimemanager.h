#ifndef DATETIMEMANAGER_H
#define DATETIMEMANAGER_H

#include <QObject>
#include <QDate>
#include <QList>
#include <Qvector>
#include <QTime>
#include <QDateTime>

class DateTimeManager : public QObject
{
    Q_OBJECT
public:
    explicit DateTimeManager(QObject *parent = nullptr);
    static QVector<QTime> allClassTimes;
    static int getTodayWeekLogical();
    static void setTodayWeek(int day);
    static const QVector<QTime>& getAllClassBeginTimes();
    static int getClassTimeIndex(QTime classTime);
    static QTime getMostLikelyClassBeginTimes();
    static void init();
private:
    static int todayWeek;
    static QDateTime lastUpdateTodayWeekTime;
    static void updateTodayWeek();
    static QTime findClosesTime(const QVector<QTime>& times,const QTime& targetTime);


signals:
};

#endif // DATETIMEMANAGER_H
