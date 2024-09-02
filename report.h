#ifndef REPORT_H
#define REPORT_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QStandardItemModel>
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
    QVector<QString> stuViewColumnNames{"姓名","课程","单价","节数","金额"};
    enum StuViewColumn{
        NAME,
        COURSE,
        PRICE,
        AMOUNT,
        STU_VIEW_TOTAL
    };

private:
private slots:
    void on_pushButton_last_year_clicked();

    void on_pushButton_this_year_clicked();

private:
    Ui::Report *ui;
    QStandardItemModel* monthViewModel;
    QStandardItemModel* stuViewModel;
};

#endif // REPORT_H
