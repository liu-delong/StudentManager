#include"dbmapper.h"
#include <QTime>
QString dbDataToString(const FieldMap &fieldMap, const QVariant &data)
{
    QString result;
    switch (fieldMap.type) {
    case DbField::INT:
        result = QString::number(data.toInt());
        break;
    case DbField::STRING:
        result = data.toString();
        break;
    case DbField::DOUBLE:
        result = QString::number(data.toDouble());
        break;
    case DbField::TIME:
        result = data.toTime().toString("hh:mm:ss");
        break;
    case DbField::DATE:
        result = data.toDate().toString("yyyy-MM-dd");
        break;
    case DbField::DATETIME:
        result = data.toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        break;
    default:
        break;
    }
    return result;
}
