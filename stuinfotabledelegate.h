#ifndef STUINFOTABLEDELEGATE_H
#define STUINFOTABLEDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QPushButton>

class StuInfoTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StuInfoTableDelegate(QWidget *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private slots:
    void onModifyButtonClick();


signals:
    void ModifyButttonClickSignal(int stu_id);
};

#endif // STUINFOTABLEDELEGATE_H
