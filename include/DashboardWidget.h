#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>

#include "Student.h"
#include "Course.h"
#include "Score.h"

class DashboardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardWidget(QWidget* parent = nullptr);

    void setData(const QVector<Student>& students,
        const QVector<Course>& courses,
        const QVector<Score>& scores);

    void refresh();

private slots:
    void saveCourses();
    void loadCourses();

private:
    QVector<Student> m_students;
    QVector<Course> m_courses;
    QVector<Score> m_scores;

    QLabel* lblStudentCount;
    QLabel* lblCourseCount;
    QLabel* lblScoreCount;

    QTableWidget* tableTop;
    QPushButton* btnViewSchedule;
    QPushButton* btnSaveSchedule;
    QPushButton* btnLoadSchedule;

    void updateStats();
    void updateTopStudents();
};

#endif