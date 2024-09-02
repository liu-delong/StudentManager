#ifndef NEWSTUDENT_H
#define NEWSTUDENT_H

#include <QWidget>
#include <QDateTime>
#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>
namespace Ui {
class NewStudent;
}

class NewStudent : public QWidget
{
    Q_OBJECT

public:
    explicit NewStudent(QWidget *parent = nullptr);
    ~NewStudent();
signals:
    void addNewStudentSignal(QString name,QString course,int total,double price,QString introducer,QDateTime payTime,QDateTime beginTime,QString msg);
public slots:
    void onAddNewStudentResult(int type,QString errorCode);
private slots:
    void on_pushButton_clicked();

private:
    Ui::NewStudent *ui;
};

#endif // NEWSTUDENT_H
