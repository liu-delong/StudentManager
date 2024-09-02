#ifndef SIGNINMANAGER_H
#define SIGNINMANAGER_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <signintabledelegate.h>
#include <QString>
#include <QVector>
#include <QMap>
#include <QTime>
#include <QVariant>
#include <QColor>
namespace Ui {
class SignInManager;
}

class SignInManager : public QWidget
{
    Q_OBJECT

public:
    explicit SignInManager(QWidget *parent = nullptr);
    ~SignInManager();
    void addRow(QMap<QString,QVariant> data);
    void clearData();
    void signIn(int row);
public slots:
    void loadAllData(int weekday,QTime now);
    void loadAllData();
    void onSignInButtonClick(QPushButton* button);
    void onSignInFinished(int status,QDate last_sign_in_date,int sign_in_count_a_day,int total,int has_attend,int remain,int stuId,int row);

private:
    Ui::SignInManager *ui;
    QTableView *table;
    QStandardItemModel *model;
    SignInTableDelegate *delegate;
    QVector<QVector<QVariant>> dbData;
    QList<QString> headerNames{"姓名","课程","签到节数","签到","上课日期","计划周几上课","课程开始时间","备注","总节数","已上节数","剩余节数","学生id"};  //相关：signIn函数
    QList<SignInTableDelegate::ColumnType> columnType{
        SignInTableDelegate::Default,
        SignInTableDelegate::Default,
        SignInTableDelegate::SpinBox,
        SignInTableDelegate::Button,
        SignInTableDelegate::DateEdit,
        SignInTableDelegate::Default,
        SignInTableDelegate::ComboBox,
        SignInTableDelegate::Default,
        SignInTableDelegate::Default,
        SignInTableDelegate::Default,
        SignInTableDelegate::Default,
        SignInTableDelegate::Default,
    };
    void setHeader();
    void setTestData();
    void setClassBeginTimeComboBoxItems(int column);
    void fixRowData(int row);
    QList<QMap<QString,QVariant>> allData;
public slots:
    void on_signInRecordDeleted(int recordId,int stu_id);
    void on_classScheduleChanged();
private slots:
    void on_pushButton_flash_clicked();
signals:
    void stuSignIned(int stu_id,int has_attend,int remain);
};

#endif // SIGNINMANAGER_H
