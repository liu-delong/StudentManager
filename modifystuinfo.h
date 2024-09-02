#ifndef MODIFYSTUINFO_H
#define MODIFYSTUINFO_H

#include <QWidget>
#include <QString>
#include <QVariant>
#include <QDateTime>

namespace Ui {
class ModifyStuInfo;
}

class ModifyStuInfo : public QWidget
{
    Q_OBJECT

public:
    explicit ModifyStuInfo(QWidget *parent = nullptr);
    void setData(int stu_id,QString name,QString course,int total,double price,QString introducer,QDateTime payTime,QDateTime beginTime,QString msg);
    void setModifyStuInfoResult(int type,QString errorCode);
    ~ModifyStuInfo();
    int m_stu_id;
public slots:
    void on_modifyStuInfoResult(int type,QString errorCode);

private slots:
    void on_pushButton_clicked();
signals:
    void modifyStuInfoSignal(int stu_id,QString name,QString course,int total,double price,QString introducer,QDateTime payTime,QDateTime beginTime,QString msg);


private:
    Ui::ModifyStuInfo *ui;
    QString temp_name;
    QString temp_course;
    int temp_total;
    double temp_price;
    QString temp_introducer;
    QDateTime temp_payTime;
    QDateTime temp_beginTime;
    QString temp_msg;
};

#endif // MODIFYSTUINFO_H
