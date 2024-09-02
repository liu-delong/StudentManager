#include "timermanager.h"

TimerManager &TimerManager::ins()
{
    static TimerManager instance;
    return instance;
}

TimerManager::TimerManager(QObject *parent)
    : QObject{parent}
{}
