#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <sqlworker.h>
#include <QThread>

class DbManager : public QObject
{
    Q_OBJECT
public:
    static DbManager& readIns();
    static DbManager& writeIns();
    static QString ip[2];
    static int port[2];
    static QString userName[2];
    static QString password[2];
    static QString databaseName[2];
    enum Dbkind{
        READ,
        WRITE
    };
    SqlWorker *sqlWork;
    QThread *sqlThread;
    virtual ~DbManager();
    bool excuteQuery(const QString& sql,std::function<void(QSqlQuery*)> callback);
    bool excuteQuery(const QStringList& sqls,std::function<void(QVector<QSqlQuery*>)> callback);
private slots:
    void onQueryFinished(QSqlQuery* q,std::function<void(QSqlQuery*)> callback);
    void onQuerysFinished(QVector<QSqlQuery*> qs,std::function<void(QVector<QSqlQuery*>)> callback);
signals:
    void excuteQuerySignal(const QString& sql,std::function<void(QSqlQuery*)> callback);
    void excuteQuerysSignal(const QStringList& sqls,std::function<void(QVector<QSqlQuery*>)> callback);
    void connectDbSignal(QString ip,int port,QString userName,QString password,QString databaseName,QString connectionName);


private:
    QString mIp;
    int mPort;
    QString mUserName;
    QString mPassword;
    QString mDataBaseName;
    explicit DbManager(QString ip,int port,QString userName,QString password,QString dataBaseName,QString connectionName,QObject *parent = nullptr);



signals:
};

#endif // DBMANAGER_H
