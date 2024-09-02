#ifndef STUINFO_H
#define STUINFO_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QMap>
#include <QString>
#include <QVector>
#include <QVariant>
#include <QDateTime>
#include "stuinfotabledelegate.h"
namespace Ui {
class stuInfo;
}

class StuInfo : public QWidget
{
    Q_OBJECT

public:
    explicit StuInfo(QWidget *parent = nullptr);
    ~StuInfo();
    void loadData();
    QStandardItemModel* getStuInfoModel();
    int getStuIdColumn();
    int getNameClassColumn();
    int getStuIdByNameClass(QString nameClass);
    QString getNameClassByStuId(int stu_id);
    friend StuInfoTableDelegate;


private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();  //新增学员按钮

    void on_ModifyStuInfoButtonClick(int stu_id);

    void on_ModifySutInfoExecButtonClick(int stu_id,QString name,QString course,int total,double price,QString introducer,QDateTime payTime,QDateTime beginTime,QString msg);



    void on_ensureAddNewStudentButtonClick(QString name,QString course,int total,double price,QString introducer,QDateTime payTime,QDateTime beginTime,QString msg);
public slots:
    void on_signInRecordDeleted(int recordId,int stu_id);
    void on_stuSignIned(int stu_id,int has_attend,int remain);

signals:
    void newStudentAdded(int stu_id);
    void StuInfoModified(int stu_id);
    void addNewStudentResultSignal(int type,QString errorCode);
    void modifyStudentResultSignal(int type,QString errorCode);
private:
    Ui::stuInfo *ui;
    QStandardItemModel* model;
    QSortFilterProxyModel* sortModel;
    QTableView* table;
    StuInfoTableDelegate* delegate;
    QVector<QVector<QVariant>> fieldMsg = {
        {"姓名","name"}, //0
        {"课程","class"}, //1
        {"总节数","total"}, //2
        {"已上节数","has_attend"}, //3
        {"剩余节数","remain"},//4
        {"学费","price"},//5
        {"每节费用","unit_price"},//6
        {"介绍人","introducer"},//7
        {"第一次上课时间","beginTime"},//8
        {"支付时间","payTime"},//9
        {"stu_id","stu_id"},//10
        {"备注","msg"},//11
        {"姓名课程","nameClass"},//12
        {"修改","stu_id"},//13
    };
    enum fieldDataBaseName{
        NAME,
        CLASS,
        TOTAL,
        HAS_ATTEND,
        REMAIN,
        PRICE,
        UNIT_PRICE,
        INTRODUCER,
        BEGINTIME,
        PAYTIME,
        STU_ID,
        MSG,
        NAMECLASS,
        MODIFY,
    };

    QMap<QString,int> fieldMap;

    QMap<int,int> stu_id2ModelRow;

    QMap<QString,int> name_class2ModelRow;

};

#endif // STUINFO_H
