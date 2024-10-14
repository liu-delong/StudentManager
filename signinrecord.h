#ifndef SIGNINRECORD_H
#define SIGNINRECORD_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QDate>
#include <QStandardItemModel>
#include <QTableView>
#include <QVector>
#include <QString>
#include <QMap>
#include "signinrecordtabledelegate.h"
namespace Ui {
class SignInRecord;
}

class SignInRecord : public QWidget
{
    Q_OBJECT

public:
    explicit SignInRecord(QWidget *parent = nullptr);
    ~SignInRecord();
    void getSignInRecord(int stuId,QDate from,QDate to,int page);
    void getSignInRecord(QDate from,QDate to,int page);
    void getSignInRecord(int page,int page_count);
    void getSignInRecord(bool force=false);
    void addRow(QMap<QString,QVariant> rowData);
    void setStuInfoModel(QStandardItemModel* model,int nameClassColumn,int stuIdColumn);

public slots:
    void on_midNight();
private slots:
    void on_pushButton_clicked();

    void on_comboBox_editTextChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_dateEdit_userDateChanged(const QDate &date);

    void on_dateEdit_2_userDateChanged(const QDate &date);

    void on_spinBox_valueChanged(int arg1);

    void on_deleteEnsureButtonClicked(int recordId,int row);
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::SignInRecord *ui;
    QTableView *table;
    QStandardItemModel *model;
    QVector<QString> headerNames{"姓名","课程","stuId","上课日期","上课时间","签到时间","课时数","备注","recordId","删除记录"};
    int stuInfoNameClassColumn;
    int stuInfoStuIdColumn;
    QStandardItemModel *stuInfoModel;
    signInRecordTableDelegate* delegate;

signals:
    void signInRecordDeleted(int recordId,int stu_id);
};

#endif // SIGNINRECORD_H
