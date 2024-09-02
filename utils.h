#ifndef UTILS_H
#define UTILS_H

#include "qwidget.h"
#include <QObject>
#include <QLayout>

class Utils : public QObject
{
    Q_OBJECT
public:
    static Utils& ins();
    void clearLayout(QLayout *layout);

    template <typename T>
    QList<T> removeDuplicates(QList<T> list);

    template <typename T>
    void replaceWidgetAt(T* layout, int index, QWidget* newWidget);

private:
    explicit Utils(QObject *parent = nullptr);

signals:
};

template<typename T>
QList<T> Utils::removeDuplicates(QList<T> list)
{
    QSet<T> set = QSet<T>::fromList(list);
    return set.toList();
}

template<typename T>
void Utils::replaceWidgetAt(T *layout, int index, QWidget *newWidget)
{
    if (index < 0 || index >= layout->count()) return; // 检查索引是否有效

    // 获取旧的widget
    QLayoutItem* item = layout->itemAt(index);
    if (item) {
        QWidget* oldWidget = item->widget();
        if (oldWidget) {
            // 从布局中移除旧widget
            layout->removeWidget(oldWidget);

            // 删除旧widget
            oldWidget->deleteLater();
        }
    }

    // 在指定索引处插入新widget
    layout->insertWidget(index, newWidget);
}

#endif // UTILS_H
