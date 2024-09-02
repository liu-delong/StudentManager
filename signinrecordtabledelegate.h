#ifndef SIGNINRECORDTABLEDELEGATE_H
#define SIGNINRECORDTABLEDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>

class signInRecordTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit signInRecordTableDelegate(QWidget *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    // void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    // void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void deleteEnsureButtonClickedSignal(int recordId,int row);
private slots:
    void onDeleteButtonClick();
};

#endif // SIGNINRECORDTABLEDELEGATE_H
