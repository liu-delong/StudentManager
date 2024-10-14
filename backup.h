#ifndef BACKUP_H
#define BACKUP_H

#include <QWidget>
#include <QThread>
namespace Ui {
class Backup;
}

class Backup : public QWidget
{
    Q_OBJECT

public:
    explicit Backup(QWidget *parent = nullptr);
    void backupBackground();
    ~Backup();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Backup *ui;
    bool isEmptyDirectory(const QString &dirpath);
};

#endif // BACKUP_H
