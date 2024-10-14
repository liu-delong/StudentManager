#include "backup.h"
#include "ui_backup.h"
#include <QFileDialog>
#include <QDir>
#include <QThread>
Backup::Backup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Backup)
{
    ui->setupUi(this);
}

void Backup::backupBackground()
{
    //开启一个子线程，调用backupFunc()进行实际备份工作。

}

Backup::~Backup()
{
    delete ui;
}

void Backup::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("备份目录,请选择空目录"),
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        if(!isEmptyDirectory(dir)){
            ui->label_hint_backup->setText("请选择一个空目录或者先前备份的、没有人为修改过的目录");
        }
        else{
            ui->lineEdit->setText(dir);
        }
    }
}

bool Backup::isEmptyDirectory(const QString &dirpath)
{
    QDir dir(dirpath);
    dir.setFilter(QDir::NoDotAndDotDot|QDir::AllEntries);
    return dir.entryList().isEmpty();
}

