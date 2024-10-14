#ifndef BACKUPTHREAD_H
#define BACKUPTHREAD_H

#include <QObject>
#include <QThread>
#include <QSharedPointer>
#include <qsqldatabase.h>
#include "dbmapper.h"
#include <QFile>
class BackupThread : public QThread
{
    Q_OBJECT
public:
    enum BackupStage{
        STUINFO,
        CLASSTME,
        LEAVE,
        MAKEUP,
        TIMEADJ,
        SIGNIN_RECORD,

    };
    enum StageStatus{
        READING,
        READ_OK,
        WRITING,
        WRITE_OK,
        READ_FAILED,
        WRITE_FAILED
    };


    static BackupThread* getInstance();
public slots:
    bool connectDb();
    void do_backup(QString folder);
    int validateFolder(QString folder);
private:

    QString stuInfoBackupFileName = "stuInfo.csv";
    enum StuInfoField{
        STUINFO_NAME=0,
        STUINFO_COURSE,
        STUINFO_TOTAL,
        STUINFO_PRICE,
        STUINFO_INTRODUCER,
        STUINFO_PAYTIME,
        STUINFO_BEGIN_TIME,
        STUINFO_HAS_ATTEND,
        STUINFO_STU_ID,
        STUINFO_MSG,
    };
    QVector<FieldMap> stuInfo_fieldMaps{
        {STUINFO_NAME,"姓名",DbField::StuInfo::name,DbField::STRING},
        {STUINFO_COURSE,"课程",DbField::StuInfo::course,DbField::STRING},
        {STUINFO_TOTAL,"总学时",DbField::StuInfo::total,DbField::INT},
        {STUINFO_PRICE,"学费",DbField::StuInfo::price,DbField::DOUBLE},
        {STUINFO_INTRODUCER,"介绍人",DbField::StuInfo::introducer,DbField::STRING},
        {STUINFO_PAYTIME,"缴费时间",DbField::StuInfo::payTime,DbField::DATETIME},
        {STUINFO_BEGIN_TIME,"开始上课时间",DbField::StuInfo::beginTime,DbField::DATETIME},
        {STUINFO_HAS_ATTEND,"已上课程",DbField::StuInfo::has_attend,DbField::INT},
        {STUINFO_STU_ID,"stu_id",DbField::StuInfo::stu_id,DbField::INT},
        {STUINFO_MSG,"备注",DbField::StuInfo::msg,DbField::STRING},
    };

    QSqlDatabase db;
    BackupStage currentStage;
    StageStatus currentStageStatus;
    int currentAmount;
    explicit BackupThread(QObject *parent = nullptr);
    QSqlQuery* excuteQuery(QString sql);
    bool backupStuInfo(QString folder);
    bool validateStuInfo(QString folder);
    void setCurrentStatus(BackupStage stage,StageStatus status,int amount=0);
    QString buildHeader(const QVector<FieldMap>& fieldMaps);
    QString buildFormatString(const QVector<FieldMap>& fieldMaps);
    bool writeQueryResultToFile(QSqlQuery* query,QVector<FieldMap>& fieldMaps,QFile &file);
signals:
    void backupFinished(int code ,QString text);
    void backupStatusChanged(int stage,int stageStatus,int amount);


signals:
};

#endif // BACKUPTHREAD_H
