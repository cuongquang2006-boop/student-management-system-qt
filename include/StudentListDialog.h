#ifndef STUDENTLISTDIALOG_H
#define STUDENTLISTDIALOG_H

#include <QDialog>
#include <QVector>
#include "Course.h"
#include "Student.h"

class QTableWidget;

class StudentListDialog : public QDialog
{
    Q_OBJECT

public:
    StudentListDialog(const Course& course,
        const QVector<Student>& students,
        QWidget* parent = nullptr);

private:
    Course m_course;
    QVector<Student> m_students;

    QTableWidget* table;

    void loadData();

private slots:
    void onViewStudent();
};

#endif