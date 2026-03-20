#include "MainWindow.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QStatusBar>
#include <QMessageBox>

#include <QPropertyAnimation>
#include <QEasingCurve>

// 🔥 JSON
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

// ================= HELPER =================
void MainWindow::setActiveButton(QPushButton* btn)
{
    for (QPushButton* b : menuButtons)
    {
        b->setStyleSheet(R"(
            QPushButton{
                text-align:left;
                padding:10px;
                border:none;
                color:white;
                font-size:15px;
                border-radius:6px;
                background:transparent;
            }

            QPushButton:hover{
                background:#2d7df6;
            }
        )");
    }

    // ACTIVE
    btn->setStyleSheet(R"(
        QPushButton{
            text-align:left;
            padding:10px;
            border:none;
            color:white;
            font-size:15px;
            border-radius:6px;
            background:#2d7df6;
        }
    )");
}

// ================= CONSTRUCTOR =================
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* central = new QWidget;
    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QFrame* sideBar = new QFrame;
    sideBar->setFixedWidth(200);

    QVBoxLayout* sideLayout = new QVBoxLayout(sideBar);
    sideLayout->setSpacing(10);
    sideLayout->setContentsMargins(10, 20, 10, 10);

    QPushButton* btnDashboard = new QPushButton("Dashboard");
    QPushButton* btnStudent = new QPushButton("Student Manager");
    QPushButton* btnCourses = new QPushButton("Courses");
    QPushButton* btnScores = new QPushButton("Scores");
    QPushButton* btnPayment = new QPushButton("Payment");

    QPushButton* btnSave = new QPushButton("Save All");
    QPushButton* btnLoad = new QPushButton("Load All");

    sideLayout->addWidget(btnDashboard);
    sideLayout->addWidget(btnStudent);
    sideLayout->addWidget(btnCourses);
    sideLayout->addWidget(btnScores);
    sideLayout->addWidget(btnPayment);

    sideLayout->addWidget(btnSave);
    sideLayout->addWidget(btnLoad);

    sideLayout->addStretch();

    // 🔥 LƯU BUTTON
    menuButtons = {
        btnDashboard,
        btnStudent,
        btnCourses,
        btnScores,
        btnPayment
    };

    // ===== STACK =====
    stack = new QStackedWidget;

    dashboardPage = new QWidget;
    QVBoxLayout* dashboardLayout = new QVBoxLayout(dashboardPage);
    QLabel* welcome = new QLabel("Dashboard");
    welcome->setStyleSheet("font-size:24px;font-weight:bold;");
    dashboardLayout->addWidget(welcome);
    dashboardLayout->addStretch();
    stack->addWidget(dashboardPage);

    // Student
    studentWidget = new StudentManagerWidget;
    connect(studentWidget, &StudentManagerWidget::backRequested,
        this, &MainWindow::openDashboard);
    stack->addWidget(studentWidget);

    // Course
    courseWidget = new CoursesManagerWidget;
    stack->addWidget(courseWidget);

    // 🔥 SYNC DATA
    connect(studentWidget, &StudentManagerWidget::dataChanged,
        this, [this]()
        {
            courseWidget->setStudentList(studentWidget->getStudents());
        });

    courseWidget->setStudentList(studentWidget->getStudents());

    mainLayout->addWidget(sideBar);
    mainLayout->addWidget(stack);

    setCentralWidget(central);

    // ===== BUTTON CONNECT =====
    connect(btnDashboard, &QPushButton::clicked, this, [=]() {
        openDashboard();
        setActiveButton(btnDashboard);
        });

    connect(btnStudent, &QPushButton::clicked, this, [=]() {
        openStudent();
        setActiveButton(btnStudent);
        });

    connect(btnCourses, &QPushButton::clicked, this, [=]() {
        openCourses();
        setActiveButton(btnCourses);
        });

    connect(btnScores, &QPushButton::clicked, this, [=]() {
        openScores();
        setActiveButton(btnScores);
        });

    connect(btnPayment, &QPushButton::clicked, this, [=]() {
        openPayment();
        setActiveButton(btnPayment);
        });

    // SAVE LOAD
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::saveAll);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadAll);

    sideBar->setStyleSheet(R"(
        QFrame{ background:#1f1f1f; }
    )");

    statusBar()->showMessage("Ready");
    resize(1000, 650);

    // 🔥 DEFAULT ACTIVE
    setActiveButton(btnDashboard);
}

// ================= SAVE =================
void MainWindow::saveAll()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save All", "", "*.json");
    if (fileName.isEmpty()) return;

    QJsonObject root;

    QJsonArray studentArr;
    for (const Student& s : studentWidget->getStudents())
        studentArr.append(s.toJson());

    root["students"] = studentArr;

    QJsonArray courseArr;
    for (const Course& c : courseWidget->getCourses())
        courseArr.append(c.toJson());

    root["courses"] = courseArr;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error", "Cannot save file!");
        return;
    }

    file.write(QJsonDocument(root).toJson());
    file.close();

    studentWidget->setModified(false);

    QMessageBox::information(this, "OK", "Saved ALL data!");
}

// ================= LOAD =================
void MainWindow::loadAll()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load All", "", "*.json");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error", "Cannot open file!");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();

    QVector<Student> students;
    for (auto v : root["students"].toArray())
        students.append(Student::fromJson(v.toObject()));

    studentWidget->setStudents(students);

    QVector<Course> courses;
    for (auto v : root["courses"].toArray())
        courses.append(Course::fromJson(v.toObject()));

    courseWidget->setCourses(courses);

    courseWidget->setStudentList(students);

    studentWidget->setModified(false);

    QMessageBox::information(this, "OK", "Loaded ALL data!");
}

// ================= CLOSE EVENT =================
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (studentWidget->isModified())
    {
        auto reply = QMessageBox::question(
            this,
            "Confirm Exit",
            "You have unsaved changes. Exit anyway?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::No)
        {
            event->ignore();
            return;
        }
    }

    event->accept();
}

// ================= NAVIGATION =================
void MainWindow::openDashboard()
{
    stack->setCurrentWidget(dashboardPage);
}

void MainWindow::openStudent()
{
    stack->setCurrentWidget(studentWidget);

    studentWidget->move(stack->width(), 0);

    QPropertyAnimation* anim = new QPropertyAnimation(studentWidget, "pos");
    anim->setDuration(220);
    anim->setStartValue(studentWidget->pos());
    anim->setEndValue(QPoint(0, 0));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::openCourses()
{
    stack->setCurrentWidget(courseWidget);

    courseWidget->move(stack->width(), 0);

    QPropertyAnimation* anim = new QPropertyAnimation(courseWidget, "pos");
    anim->setDuration(220);
    anim->setStartValue(courseWidget->pos());
    anim->setEndValue(QPoint(0, 0));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::openScores()
{
    QMessageBox::information(this, "Scores", "Scores module is not implemented yet.");
}

void MainWindow::openPayment()
{
    QMessageBox::information(this, "Payment", "Payment module is not implemented yet.");
}