#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QCloseEvent>
#include <QVector>
#include <QPushButton>

#include "StudentManagerWidget.h"
#include "CoursesManagerWidget.h"
#include "Student.h"
#include "Course.h"  // nếu Course class cần dùng trong save/load

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

    CoursesManagerWidget* courseWidget;

private:
    QStackedWidget* stack;

    QWidget* dashboardPage;
    StudentManagerWidget* studentWidget;

    // 🔥 MENU HIGHLIGHT
    QVector<QPushButton*> menuButtons;
    void setActiveButton(QPushButton* btn);

    // 🔥 SAVE / LOAD
    void saveAll();
    void loadAll();

    void closeEvent(QCloseEvent* event);

private slots:
    void openDashboard();
    void openStudent();
    void openCourses();
    void openScores();
    void openPayment();
};

#endif