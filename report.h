#ifndef REPORT_H
#define REPORT_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
namespace Ui {
class Report;
}

class Report : public QWidget
{
    Q_OBJECT

public:
    explicit Report(QWidget *parent = nullptr);
    ~Report();
    QVector<QString> monthViewColumnNames{"月份","核销总金额"};
    enum MonthViewColumn{
        MONTH,
        MONTH_VIEW_TOTAL
    };
    QVector<QString> stuViewColumnNames{"stu_id","姓名","课程","单价","节数","金额"};
    enum StuViewColumn{
        STU_ID,
        NAME,
        COURSE,
        PRICE,
        AMOUNT,
        STU_VIEW_TOTAL
    };
public slots:
    void loadMonthViewData();
    void loadStuViewData();
    void on_pushButton_flash_month_view_clicked();
    void on_pushButton_flash_stu_view_clicked();

private:
private slots:
    void on_pushButton_last_year_clicked();

    void on_pushButton_this_year_clicked();





    void on_dateEdit_year_dateChanged(const QDate &date);

private:
    Ui::Report *ui;
    QStandardItemModel* monthViewModel;
    QStandardItemModel* stuViewModel;
    QSortFilterProxyModel* stuViewSortModel;
};

#endif // REPORT_H
