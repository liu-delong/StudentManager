#ifndef DBMAPPER_H
#define DBMAPPER_H
#include <QString>
#include <QMap>
#include <QVariant>
namespace DbField {
    enum FieldType{
        INT,
        DOUBLE,
        DATETIME,
        DATE,
        TIME,
        STRING
    };
    namespace StuInfo {
        const char* name = "name";
        const char* course = "class";
        const char* total = "total";
        const char* price = "price";
        const char* introducer = "introducer";
        const char* payTime = "payTime";
        const char* beginTime = "beginTime";
        const char* has_attend = "has_attend";
        const char* stu_id = "stu_id";
        const char* last_sign_in_date = "last_sign_in_date";
        const char* sign_in_count_a_day = "sign_in_count_a_day";
        const char* msg = "msg";
    }


}
struct FieldMap{
    int code;
    QString displayName;
    QString dbFieldName;
    DbField::FieldType type;
};

QString dbDataToString(const FieldMap& fieldMap,const QVariant& data);


#endif // DBMAPPER_H
