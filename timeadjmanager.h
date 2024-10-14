#ifndef TIMEADJMANAGER_H
#define TIMEADJMANAGER_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStringList>

namespace Ui {
class TimeAdjManager;
}

class TimeAdjManager : public QWidget
{
    Q_OBJECT

public:
    explicit TimeAdjManager(QWidget *parent = nullptr);
    ~TimeAdjManager();
    QStringList columnNames{"调休日期","调休为星期几"};
    enum ColumnName{
        ADJTIME,
        ADJTO,
    };
public slots:
    void on_midNight();


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::TimeAdjManager *ui;
    QStandardItemModel *model;
};

#endif // TIMEADJMANAGER_H
