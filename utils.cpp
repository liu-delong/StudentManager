#include "utils.h"

Utils &Utils::ins()
{
    static Utils utils;
    return utils;
}

void Utils::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->layout()) {
            // Recursive clearing if the item is a layout
            clearLayout(item->layout());
            delete item->layout(); // Delete the layout object itself
        } else if (item->widget()) {
            // Delete the widget if it exists
            delete item->widget();
        }
        delete item; // Delete the layout item
    }

}

Utils::Utils(QObject *parent)
    : QObject{parent}
{}


