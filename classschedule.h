#ifndef CLASSSCHEDULE_H
#define CLASSSCHEDULE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QListView>
#include <QVector>
#include <QTime>
#include <QMap>
#include <QString>
namespace Ui {
class ClassSchedule;
}
struct Command_metaData{
    int stu_id;
    int weekday;
    QTime classTime;
    bool operator <(const Command_metaData& other) const{
        if(stu_id == other.stu_id){
            if(weekday == other.weekday){
                return classTime < other.classTime;
            }
            return weekday<other.weekday;
        }
        return stu_id<other.stu_id;
    }
    operator QString() const{
        return QString("(%1 %2 %3)").arg(stu_id).arg(weekday).arg(classTime.toString("hh:mm"));
    }
};

class ClassSchedule : public QWidget
{
    Q_OBJECT

public:
    explicit ClassSchedule(QWidget *parent = nullptr);
    ~ClassSchedule();
    void setModel(QStandardItemModel* model,int column);
    void loadData();
    QStandardItemModel* trash_model;
    QVector<QVector<QListView*>> classTable;
    QVector<QVector<QStandardItemModel*>> classTableModel;
    QMap<Command_metaData,int> commands;
signals:
    void classScheduleChange();



private slots:
    void on_pushButton_edit_clicked();
    void on_rowsInserted(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void on_rowsAboutToBeRemoved(const QModelIndex &parent,int first,int last);


    void on_pushButton_flash_clicked();

private:
    Ui::ClassSchedule *ui;
    void changeToEditMode();
    void changeToSubmitMode();
    void changeToNormalMode();
    void submitChange();
};

#endif // CLASSSCHEDULE_H
