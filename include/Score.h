#ifndef SCORE_H
#define SCORE_H

#include <QString>
#include <QJsonObject>

class Score
{
public:
    QString studentId;
    QString courseId;

    double process = 0;
    double final = 0;
    double total = 0;

    Score() {}

    Score(QString sid, QString cid, double p, double f)
        : studentId(sid), courseId(cid), process(p), final(f)
    {
        total = calcTotal(p, f);
    }

    static double calcTotal(double process, double final)
    {
        return process * 0.6 + final * 0.4;
    }

    QJsonObject toJson() const
    {
        QJsonObject obj;
        obj["studentId"] = studentId;
        obj["courseId"] = courseId;
        obj["process"] = process;
        obj["final"] = final;
        obj["total"] = total;
        return obj;
    }

    static Score fromJson(const QJsonObject& obj)
    {
        Score s;
        s.studentId = obj["studentId"].toString();
        s.courseId = obj["courseId"].toString();
        s.process = obj["process"].toDouble();
        s.final = obj["final"].toDouble();
        s.total = obj["total"].toDouble();

        if (s.total == 0)
            s.total = calcTotal(s.process, s.final);

        return s;
    }
};

#endif