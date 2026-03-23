#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

#include <QDialog>
#include <QVector>
#include "Course.h"

class QTableWidget;

class ScheduleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScheduleDialog(QWidget* parent = nullptr);

    void setSchedule(const QVector<Course>& courses);

private:
    QTableWidget* table;
};

#endif