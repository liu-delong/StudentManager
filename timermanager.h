#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <QObject>
#include <vector>
#include <QTimer>
#include <QVariant>
struct TimerTask{
    int interval;
    std::function<void()> function;
    TimerTask(int _interval,std::function<void()> func):interval(_interval),function(func){}
};

class TimerManager : public QObject
{
    Q_OBJECT
public:
    static TimerManager& ins();
    template<typename Callable,typename... Args>
    int registerTask(int interval_s,Callable&& func,Args&&... args){
        auto f = std::bind(std::forward<Callable>(func),std::forward<Args>(args)...);
        tasks.emplace_back(interval_s,f);
        int index = tasks.size()-1;
        startTimer(index);
        return index;
    }
    bool removeTask(unsigned long long index){
        if(index>=timers.size()){
            return false;
        }
        else {
            timers[index]->deleteLater();
            timers.erase(timers.begin()+index);
            tasks.erase(tasks.begin()+index);
            return true;
        }
    }
private:
    explicit TimerManager(QObject *parent = nullptr);
    std::vector<TimerTask> tasks;
    std::vector<QTimer*> timers;
    void startTimer(int taskIndex){
        QTimer* timer = new QTimer(this);
        timers.push_back(timer);
        timer->setProperty("taskIndex",taskIndex);
        connect(timer,SIGNAL(timeout()),this,SLOT(excuteTask()));
        timer->start(tasks[taskIndex].interval*1000);
    }
private slots:
    void excuteTask(){
        auto taskIndex = sender()->property("taskIndex").toInt();
        tasks[taskIndex].function();
    }

signals:
};

#endif // TIMERMANAGER_H
