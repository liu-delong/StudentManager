#ifndef DATETIMEMANAGER_H
#define DATETIMEMANAGER_H

#include <QObject>
#include <QDate>
#include <QList>
#include <Qvector>
#include <QTime>
#include <QDateTime>
#include <QTimer>

class DateTimeManager : public QObject
{
    Q_OBJECT
public:

    static DateTimeManager& getInstance();
    int getTodayWeekLogical();
    void setTodayWeek(int day);
    const QVector<QTime>& getAllClassBeginTimes();
    int getClassTimeIndex(QTime classTime);
    void init();
    void updateTodayWeek();
    void sendSignalEveryMidnight();
public slots:
    void on_MidNight();
signals:
    void todayWeekchange(int weekday);
    void midNightArrived();
private:
    int todayWeek;
    QVector<QTime> allClassTimes{QTime::fromString("08:30"),QTime::fromString("10:30"),QTime::fromString("14:30")
                                 ,QTime::fromString("16:30"),QTime::fromString("17:00"),QTime::fromString("19:15")};;
    QTimer *firstTimer;
    QTimer *dailyTimer;
    explicit DateTimeManager(QObject *parent = nullptr);
};

#endif // DATETIMEMANAGER_H
