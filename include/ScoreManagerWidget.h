#ifndef SCOREMANAGERWIDGET_H
#define SCOREMANAGERWIDGET_H

#include <QWidget>
#include <QVector>
#include "Score.h"
#include "Student.h"
#include "Course.h"

class QTableWidget;
class QComboBox;
class QLineEdit;

class ScoreManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScoreManagerWidget(QWidget* parent = nullptr);

    void setData(const QVector<Student>& students,
        const QVector<Course>& courses,
        const QVector<Score>& scores);

    const QVector<Score>& getScores() const;

private:
    QVector<Student> studentList;
    QVector<Course> courseList;
    QVector<Score> scoreList;

    QTableWidget* table;

    QComboBox* courseBox;
    QComboBox* studentBox;

    QLineEdit* processEdit;
    QLineEdit* finalEdit;

    void setupUI();
    void setupConnections();

    void refreshTable();
    void updateStudentBox();

private slots:
    void addOrUpdateScore();
    void deleteScore();
    void onCourseChanged(int);
};

#endif