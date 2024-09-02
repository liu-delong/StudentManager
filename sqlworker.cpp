#include "sqlworker.h"
#include <QDebug>
#include <QSqlError>
SqlWorker::SqlWorker(QObject* parent):QObject(parent){}

void SqlWorker::excuteQuery(const QString &sql, std::function<void (QSqlQuery *)> callback)
{
    QMutexLocker locker(&sqlMutex);
    QSqlQuery *q = new QSqlQuery(db);
    if(!q->exec(sql)){
        auto error = q->lastError();
        qDebug()<<"[SqlWorker::excuteQuery] query "<<sql<<" error:("<<error.nativeErrorCode()<<") "<<error.text();error.type();
    }
    emit queryFinished(q,callback);
}

void SqlWorker::connectDatabase(QString ip, int port, QString userName, QString password, QString databaseName,QString connectionName)
{
    db = QSqlDatabase::addDatabase("QMYSQL",connectionName);
    db.setHostName(ip);
    db.setPort(port);
    db.setUserName(userName);
    db.setPassword(password);
    db.setDatabaseName(databaseName);
    if(!db.open()){
        qDebug() << "Failed to connect to database";
    }
}

void SqlWorker::excuteQuerys(const QStringList &sqls, std::function<void (QVector<QSqlQuery *>)> callback)
{
    QMutexLocker locker(&sqlMutex);
    QVector<QSqlQuery *> qs(sqls.count());
    for(int i=0;i<sqls.size();i++){
        qs[i] = new QSqlQuery(db);
        if(!qs[i]->exec(sqls[i])){
            auto error = qs[i]->lastError();
            qDebug()<<"[SqlWorker::excuteQuery] query "<<sqls[i]<<" error:("<<error.nativeErrorCode()<<") "<<error.text();error.type();
        }
    }
    emit querysFinished(std::move(qs),callback);
}
