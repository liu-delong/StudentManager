#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "signinmanager.h"
#include "signinrecord.h"
#include "stuinfo.h"
#include "classschedule.h"
#include "leaveandmakeup.h"
#include "datetimemanager.h"
#include "timeadjmanager.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
void showHint(QString msg);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    static MainWindow& getInstance();
    ~MainWindow();
    SignInManager* signInManager;
    SignInRecord* signInRecord;
    StuInfo* stuInfo;
    ClassSchedule* classSchedule;
    LeaveAndMakeup* leaveAndMakeup;
    DateTimeManager* dateTimeManager;
    TimeAdjManager* timeAdjManager;

    void showHint(QString msg);

public slots:
    void on_midNight();



private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_classSchedule_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();


    void on_pushButton_6_clicked();

private:
    MainWindow(QWidget *parent = nullptr);
    Ui::MainWindow *ui;
    QTimer *dailyTimer;
    QTimer* firstTimer;

signals:
    void midNightSignal();
};
#endif // MAINWINDOW_H
