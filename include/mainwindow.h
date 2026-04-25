#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QCloseEvent>
#include <QVector>
#include <QPushButton>

#include "StudentManagerWidget.h"
#include "CoursesManagerWidget.h"
#include "ScoreManagerWidget.h"
#include "Student.h"
#include "Course.h"
#include "DashboardWidget.h"   

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    CoursesManagerWidget* courseWidget;
    StudentManagerWidget* studentWidget;
    ScoreManagerWidget* scoreWidget;
    DashboardWidget* dashboardWidget; 

private:
    QStackedWidget* stack;

    QVector<QPushButton*> menuButtons;
    void setActiveButton(QPushButton* btn);

    void saveAll();
    void loadAll();

    void closeEvent(QCloseEvent* event);

private slots:
    void openDashboard();
    void openStudent();
    void openCourses();
    void openScores();
};

#endif
