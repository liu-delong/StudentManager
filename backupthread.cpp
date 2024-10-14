#include "backupthread.h"
#include "dbmanager.h"
#include <QDebug>
#include "mainwindow.h"
#include <QSqlError>
#include <QDateTime>
#include <QFile>
#include <QDir>
BackupThread::BackupThread(QObject *parent)
    : QThread{parent}
{}

QSqlQuery *BackupThread::excuteQuery(QString sql)
{
    int retryTime = 1;
    auto query = new QSqlQuery(db);
    while(retryTime>=0){
        if(query->exec(sql)){
            return query;
        }
        else{
            QSqlError error = query->lastError();
            auto type = error.type();
            auto code = error.nativeErrorCode();
            auto text= error.text();
            qDebug()<<"BackupThread::excuteQuery:query error: "<<type<<" "<<code<<" "<<text;
            if(type == QSqlError::ConnectionError){ //数据库断开连接，重新连接数据库并执行查询。
                qDebug()<<"BackupThread::excuteQuery：数据库断开连接，尝试重连";
                if(connectDb()){
                    delete query;
                    query = new QSqlQuery(db);
                    retryTime -=1;
                    qDebug()<<"BackupThread::excuteQuery:数据库重连成功,重新执行查询";
                }
                else{
                    qDebug()<<"BackupThread::excuteQuery:数据库重连失败";
                    return query;
                }
            }
            else{
                return query;
            }
        }
    }
    return query;
}

bool BackupThread::backupStuInfo(QString folder)
{
    setCurrentStatus(STUINFO,READING,0);
    QString sql = "select * from stu_info";
    auto query = excuteQuery(sql);
    if(!query->isActive()){
        qDebug()<<"查询stuInfo失败";
        setCurrentStatus(STUINFO,READ_FAILED,0);
        return false;
    }
    int amount = query->size();
    setCurrentStatus(STUINFO,READ_OK,amount);
    QDir backupFolder(folder);
    QFile file(backupFolder.filePath(stuInfoBackupFileName));
    bool openOk = file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    if(openOk){
        setCurrentStatus(STUINFO,WRITING);
    }
    else{
        setCurrentStatus(STUINFO,WRITE_FAILED);
        return false;
    }
    return writeQueryResultToFile(query,stuInfo_fieldMaps,file);
}

bool BackupThread::validateStuInfo(QString)
{

    return true;
}

void BackupThread::setCurrentStatus(BackupStage stage, StageStatus status,int amount)
{
    bool hasChanged = currentStage != stage || currentStageStatus != status || amount != currentAmount;
    currentStage = stage;
    currentStageStatus = status;
    currentAmount = amount;
    if(hasChanged){
        emit backupStatusChanged(currentStage,currentStageStatus,amount);
    }
}

QString BackupThread::buildHeader(const QVector<FieldMap> &fieldMaps)
{
    QString header;
    for(const auto&fieldMap:fieldMaps){
        QString displayName = fieldMap.displayName;
        if(header!=""){
            header+=",";
        }
        header+=displayName;
    }
    return header;

}

QString BackupThread::buildFormatString(const QVector<FieldMap> &fieldMaps)
{
    QString formatString;
    for(int i=0;i<fieldMaps.size();i++){
        if(formatString!=""){
            formatString+=",";
        }
        if(fieldMaps[i].type == DbField::STRING){
            formatString+= QString("\"%")+QString::number(i+1)+QString("\"");
        }
        else{
            formatString+= QString("%")+QString::number(i+1);
        }
    }
    return formatString;
}

bool BackupThread::writeQueryResultToFile(QSqlQuery *query, QVector<FieldMap>& fieldMaps, QFile &file)
{
    QString header = buildHeader(fieldMaps);
    int result = file.write(header.toUtf8());
    if(result<0){
        return false;
    }
    QString lineFormat = buildFormatString(fieldMaps);
    int count =0;
    while(query->next()){
        QString lineData = lineFormat;
        for(int i=0;i<fieldMaps.size();i++){
            const auto& fieldMap = fieldMaps[i];
            auto fieldData = query->value(fieldMap.dbFieldName);
            lineData = lineData.arg(dbDataToString(fieldMap,fieldData));
        }
        result = file.write(lineData.toUtf8());
        if(result<0){
            qDebug()<<"BackupThread::writeQueryResultToFile:"<<QString("write %1 line failed").arg(count+1);
            return false;
        }
        count += 1;
        if(count%100 == 0){
            qDebug()<<"BackupThread::writeQueryResultToFile:"<<QString("has write %1 lines").arg(count);
        }
    }
    return true;
}

BackupThread *BackupThread::getInstance()
{
    static BackupThread ins;
    return &ins;
}

bool BackupThread::connectDb()
{
    if(db.isOpen()) return true;
    QString ip = DbManager::ip[DbManager::READ];
    int port = DbManager::port[DbManager::READ];
    QString userName = DbManager::userName[DbManager::READ];
    QString password = DbManager::password[DbManager::READ];
    QString databaseName = DbManager::databaseName[DbManager::READ];
    db = QSqlDatabase::addDatabase("QMYSQL","backup");
    db.setHostName(ip);
    db.setPort(port);
    db.setUserName(userName);
    db.setPassword(password);
    db.setDatabaseName(databaseName);
    if(!db.open()){
        QSqlError error = db.lastError();
        MainWindow::getInstance().showHint(QString("备份：连接数据库失败 type %1 code %2 text %3").arg(error.type()).arg(error.nativeErrorCode()).arg(error.text()));
        qDebug() << "Failed to connect to database";
        return false;
    }
    return true;
}


void BackupThread::do_backup(QString folder)
{
    connectDb();
    int validateResult = validateFolder(folder);
    if(validateResult<0){
        emit backupFinished(validateResult,"文件夹检查错误");
        return;
    }



}

int BackupThread::validateFolder(QString )
{
    return 0;
}



