#ifndef LEAVEANDMAKEUP_H
#define LEAVEANDMAKEUP_H

#include <QWidget>
#include <QStandardItemModel>
#include <QVector>
#include <QString>
#include <QTableView>
#include <QItemDelegate>

namespace Ui {
class LeaveAndMakeup;
}


class LeaveAndMakeup : public QWidget
{
    Q_OBJECT

public:
    explicit LeaveAndMakeup(QWidget *parent = nullptr);
    ~LeaveAndMakeup();
    void setStuInfoModel(QStandardItemModel* model,int nameClassColumn,int stuIdColumn);
    QVector<QString> columnNames{"姓名","课程","日期","课程时间","删除","stu_id"};
    enum columnId{
        NAME,
        COURSE,
        DATE,
        TIME,
        REMOVE,
        STU_ID
    };
    enum WhichTable{
        LEAVE,
        MAKEUP,
        ALL
    };

public slots:
    void loadData(WhichTable whichTable = ALL);


private slots:
    void on_pushButton_leave_clicked();

    void on_pushButton_makeUp_clicked();

    void on_deleteButtonClicked(int whichType,int row);

    void on_pushButton_flash_clicked();

private:
    Ui::LeaveAndMakeup *ui;
    int m_nameClassColumn;
    int m_stuIdColumn;
    QStandardItemModel* m_stuInfoModel;
    QStandardItemModel* leave_model;
    QStandardItemModel* makeUp_model;
    QTableView* leave_table;
    QTableView* makeUp_table;
};

#endif // LEAVEANDMAKEUP_H
