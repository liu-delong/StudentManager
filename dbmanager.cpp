#include "dbmanager.h"
#include <QDebug>
QString DbManager::ip[2]={"127.0.0.1","127.0.0.1"};
int DbManager::port[2]={3306,3306};
QString DbManager::userName[2]={"root","root"};
QString DbManager::password[2]={"Wn9323252766","Wn9323252766"};
QString DbManager::databaseName[2]={"Stu","Stu"};

DbManager& DbManager::readIns()
{
    static DbManager ins(ip[READ],port[READ],userName[READ],password[READ],databaseName[READ],"read");
    return ins;
}

DbManager &DbManager::writeIns()
{
    static DbManager ins(ip[WRITE],port[WRITE],userName[WRITE],password[WRITE],databaseName[WRITE],"write");
    return ins;
}

DbManager::~DbManager()
{
    sqlThread->quit();
    sqlThread->wait();
}


DbManager::DbManager(QString ip,int port,QString userName,QString password,QString dataBaseName,QString connectionName,QObject *parent)
    : QObject{parent},mIp(ip),mPort(port),mUserName(userName),mPassword(password),mDataBaseName(dataBaseName)
{
    sqlWork = new SqlWorker();
    sqlThread = new QThread(this);
    sqlWork->moveToThread(sqlThread);
    sqlThread->start();
    connect(this,&DbManager::excuteQuerySignal,sqlWork,&SqlWorker::excuteQuery);
    connect(sqlWork,&SqlWorker::queryFinished,this,&DbManager::onQueryFinished);

    connect(this,&DbManager::excuteQuerysSignal,sqlWork,&SqlWorker::excuteQuerys);
    connect(sqlWork,&SqlWorker::querysFinished,this,&DbManager::onQuerysFinished);


    connect(this,&DbManager::connectDbSignal,sqlWork,&SqlWorker::connectDatabase);
    emit connectDbSignal(mIp,mPort,mUserName,mPassword,mDataBaseName,connectionName);
}



bool DbManager::excuteQuery(const QString &sql, std::function<void (QSqlQuery *)> callback)
{
    emit excuteQuerySignal(sql,callback);
    return true;
}

bool DbManager::excuteQuery(const QStringList &sqls, std::function<void (QVector<QSqlQuery *>)> callback)
{
    emit excuteQuerysSignal(sqls,callback);
    return true;
}

void DbManager::onQueryFinished(QSqlQuery *q, std::function<void (QSqlQuery *)> callback)
{
    callback(q);
    delete q;
}

void DbManager::onQuerysFinished(QVector<QSqlQuery *> qs, std::function<void (QVector<QSqlQuery *>)> callback)
{
    callback(qs);
    for(auto q:qs){
        delete q;
    }
}

