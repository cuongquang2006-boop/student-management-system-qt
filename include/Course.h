#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

class Course
{
public:
    QString id;
    QString name;
    QString teacher;
    QString className;        //  THÊM
    QStringList studentIds;

    Course() {}

    //  constructor mới
    Course(QString id, QString name, QString teacher, QString className)
        : id(id), name(name), teacher(teacher), className(className) {
    }

    QJsonObject toJson() const
    {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["teacher"] = teacher;
        obj["class"] = className;   //  THÊM

        QJsonArray arr;
        for (const QString& sid : studentIds)
            arr.append(sid);

        obj["students"] = arr;
        return obj;
    }

    static Course fromJson(const QJsonObject& obj)
    {
        Course c(
            obj["id"].toString(),
            obj["name"].toString(),
            obj["teacher"].toString(),
            obj["class"].toString()   //  THÊM
        );

        QJsonArray arr = obj["students"].toArray();
        for (auto v : arr)
            c.studentIds.append(v.toString());

        return c;
    }
};

#endif
