
#ifndef STUDENT_H
#define STUDENT_H

#include <QString>       
#include <QJsonObject>    

class Student
{
public:

    QString id;      
    QString name;    
    int age;         
    QString gender;  
    QString major;   

    Student() {}

    Student(QString id, QString name, int age, QString gender, QString major)
        : id(id), name(name), age(age), gender(gender), major(major) {}

    QJsonObject toJson() const
    {
        QJsonObject obj;     

        obj["id"] = id;
        obj["name"] = name;
        obj["age"] = age;
        obj["gender"] = gender;
        obj["major"] = major;

        return obj; 
    }

    static Student fromJson(const QJsonObject &obj)
    {
        return Student(
            obj["id"].toString(),
            obj["name"].toString(),
            obj["age"].toInt(),
            obj["gender"].toString(),
            obj["major"].toString()
            );
    }
};

#endif
