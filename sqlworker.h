#ifndef SQLWORKER_H
#define SQLWORKER_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <functional>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
class SqlWorker:public QObject
{
    Q_OBJECT
public:
    SqlWorker(QObject* parent = nullptr);
public slots:
    void excuteQuery(const QString& sql,std::function<void(QSqlQuery*)> callback);
    void connectDatabase(QString ip,int port,QString userName,QString password,QString databaseName,QString connectionName);
    void excuteQuerys(const QStringList& sqls,std::function<void(QVector<QSqlQuery*>)> callback);
private:
    QSqlDatabase db;
    QMutex sqlMutex;
signals:
    void queryFinished(QSqlQuery* q,std::function<void(QSqlQuery*)> callback);
    void querysFinished(QVector<QSqlQuery*> qs,std::function<void(QVector<QSqlQuery*>)> callback);
};

#endif // SQLWORKER_H
