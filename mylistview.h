#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QObject>
#include <QListView>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QStandardItemModel>
#include <QDropEvent>
#include <QDebug>
class MyListView : public QListView
{
    Q_OBJECT
public:
    explicit MyListView(QWidget *parent = nullptr);
protected:
    void dragEnterEvent(QDragEnterEvent *event) override
    {
        if (event->source() == this) {
            qDebug()<<"event->source() == this";
            event->ignore();
            return;
        }
        if (event->mimeData()->hasFormat("application/x-qstandarditemmodeldatalist")) {
            QByteArray encodedData = event->mimeData()->data("application/x-qstandarditemmodeldatalist");
            QDataStream stream(&encodedData, QIODevice::ReadOnly);

            QStandardItemModel *model = qobject_cast<QStandardItemModel *>(this->model());
            if (!model) {
                event->ignore();
                return;
            }

            while (!stream.atEnd()) {
                int row, col;
                QMap<int, QVariant> roleDataMap;
                stream >> row >> col >> roleDataMap;

                // 假设要检查的是显示角色的数据
                QString itemText = roleDataMap[Qt::DisplayRole].toString();

                // 检查模型中是否已经存在该项
                QList<QStandardItem *> existingItems = model->findItems(itemText);
                if (!existingItems.isEmpty()) {
                    event->ignore(); // 如果已经存在相同项，则不接收
                    return;
                }
            }

            event->acceptProposedAction(); // 如果没有重复项，允许拖拽
        } else {
            event->ignore();
        }
    }

signals:
};

#endif // MYLISTVIEW_H
