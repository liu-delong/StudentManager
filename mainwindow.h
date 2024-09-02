#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "signinmanager.h"
#include "signinrecord.h"
#include "stuinfo.h"
#include "classschedule.h"
#include "leaveandmakeup.h"
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
    void showHint(QString msg);



private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_classSchedule_clicked();

    void on_pushButton_4_clicked();

private:
    MainWindow(QWidget *parent = nullptr);
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
