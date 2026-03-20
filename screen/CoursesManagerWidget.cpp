#include "CoursesManagerWidget.h"
#include "StudentListDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QLabel>

// ================= CONSTRUCTOR =================
CoursesManagerWidget::CoursesManagerWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

// ================= UI =================
void CoursesManagerWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // ===== FORM =====
    idEdit = new QLineEdit;
    idEdit->setPlaceholderText("Course ID");

    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("Course Name");

    teacherEdit = new QLineEdit;
    teacherEdit->setPlaceholderText("Teacher");

    classBox = new QComboBox;
    classBox->addItems({
        "<<None>>",
        "CNTT1","CNTT2","CNTT3","CNTT4",
        "SE1","SE2","SE3",
        "AI1","AI2",
        "DS1","DS2",
        "Marketing1","Marketing2",
        "Business1","Business2"
        });

    QPushButton* addBtn = new QPushButton("Add Course");
    QPushButton* deleteBtn = new QPushButton("Delete Course");

    addBtn->setObjectName("addBtn");
    deleteBtn->setObjectName("deleteBtn");

    QHBoxLayout* formLayout = new QHBoxLayout;
    formLayout->addWidget(idEdit);
    formLayout->addWidget(nameEdit);
    formLayout->addWidget(teacherEdit);
    formLayout->addWidget(classBox);
    formLayout->addWidget(addBtn);
    formLayout->addWidget(deleteBtn);

    // ===== STUDENT AREA =====
    studentBox = new QComboBox;

    QPushButton* addStudentBtn = new QPushButton("Add Student");
    QPushButton* viewStudentsBtn = new QPushButton("View Students");

    addStudentBtn->setObjectName("addStudentBtn");
    viewStudentsBtn->setObjectName("viewStudentsBtn");

    QHBoxLayout* studentLayout = new QHBoxLayout;
    studentLayout->addWidget(new QLabel("Student:"));
    studentLayout->addWidget(studentBox);
    studentLayout->addWidget(addStudentBtn);
    studentLayout->addWidget(viewStudentsBtn);

    // ===== TABLE =====
    table = new QTableWidget;
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(
        { "ID", "Name", "Teacher", "Class", "Students" });

    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // ===== ADD TO MAIN =====
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(studentLayout);
    mainLayout->addWidget(table);
}

// ================= CONNECTION =================
void CoursesManagerWidget::setupConnections()
{
    QPushButton* addBtn = findChild<QPushButton*>("addBtn");
    QPushButton* deleteBtn = findChild<QPushButton*>("deleteBtn");
    QPushButton* addStudentBtn = findChild<QPushButton*>("addStudentBtn");
    QPushButton* viewStudentsBtn = findChild<QPushButton*>("viewStudentsBtn");

    connect(addBtn, &QPushButton::clicked,
        this, &CoursesManagerWidget::addCourse);

    connect(deleteBtn, &QPushButton::clicked,
        this, &CoursesManagerWidget::deleteCourse);

    connect(addStudentBtn, &QPushButton::clicked,
        this, &CoursesManagerWidget::addStudentToCourse);

    connect(viewStudentsBtn, &QPushButton::clicked,
        this, &CoursesManagerWidget::onViewStudents);

    connect(table, &QTableWidget::cellClicked, this,
        [this](int row, int)
        {
            updateStudentCombo(row);
        });
}

// ================= SET DATA =================
void CoursesManagerWidget::setStudentList(const QVector<Student>& students)
{
    studentList = students;
}

void CoursesManagerWidget::setCourses(const QVector<Course>& list)
{
    courses = list;
    refreshTable();
}

// ================= GET =================
const QVector<Course>& CoursesManagerWidget::getCourses() const
{
    return courses;
}

const QVector<Student>& CoursesManagerWidget::getStudentList() const
{
    return studentList;
}

// ================= ADD COURSE =================
void CoursesManagerWidget::addCourse()
{
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString teacher = teacherEdit->text().trimmed();
    QString className = classBox->currentText();

    if (!isValidCourseId(id))
    {
        QMessageBox::warning(this, "Error", "Invalid ID!");
        return;
    }

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Name required!");
        return;
    }

    if (!isValidTeacherName(teacher))
    {
        QMessageBox::warning(this, "Error", "Invalid teacher!");
        return;
    }

    if (className == "<<None>>")
    {
        QMessageBox::warning(this, "Error", "Select class!");
        return;
    }

    for (const Course& c : courses)
    {
        if (c.id == id)
        {
            QMessageBox::warning(this, "Error", "Duplicate ID!");
            return;
        }
    }

    courses.append(Course(id, name, teacher, className));
    refreshTable();
}

// ================= DELETE COURSE =================
void CoursesManagerWidget::deleteCourse()
{
    int row = table->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(this, "Error", "Select a course first!");
        return;
    }

    auto reply = QMessageBox::question(
        this,
        "Confirm",
        "Delete this course?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes) return;

    courses.removeAt(row);

    refreshTable();
    studentBox->clear();
}

// ================= ADD STUDENT =================
void CoursesManagerWidget::addStudentToCourse()
{
    int row = table->currentRow();
    if (row < 0) return;

    QString studentId = studentBox->currentData().toString();

    if (courses[row].studentIds.contains(studentId))
    {
        QMessageBox::information(this, "Info", "Already added!");
        return;
    }

    courses[row].studentIds.append(studentId);

    refreshTable();
}

// ================= UPDATE COMBO =================
void CoursesManagerWidget::updateStudentCombo(int row)
{
    studentBox->clear();

    if (row < 0 || row >= courses.size()) return;

    QString cls = courses[row].className;

    for (const Student& s : studentList)
    {
        if (s.className == cls)
        {
            studentBox->addItem(s.name + " (" + s.id + ")", s.id);
        }
    }
}

// ================= REFRESH =================
void CoursesManagerWidget::refreshTable()
{
    table->setRowCount(courses.size());

    for (int i = 0; i < courses.size(); ++i)
    {
        const Course& c = courses[i];

        table->setItem(i, 0, new QTableWidgetItem(c.id));
        table->setItem(i, 1, new QTableWidgetItem(c.name));
        table->setItem(i, 2, new QTableWidgetItem(c.teacher));
        table->setItem(i, 3, new QTableWidgetItem(c.className));

        QStringList names;

        for (const QString& sid : c.studentIds)
        {
            for (const Student& s : studentList)
            {
                if (s.id == sid)
                    names.append(s.name);
            }
        }

        table->setItem(i, 4, new QTableWidgetItem(names.join(", ")));
    }
}

// ================= VALIDATION =================
bool CoursesManagerWidget::isValidCourseId(const QString& id)
{
    QRegularExpression regex("^[A-Za-z0-9]+$");
    return regex.match(id).hasMatch();
}

bool CoursesManagerWidget::isValidTeacherName(const QString& name)
{
    QRegularExpression regex("^[\\p{L} ]+$");
    return regex.match(name).hasMatch();
}

// ================= VIEW STUDENTS =================
void CoursesManagerWidget::onViewStudents()
{
    int row = table->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(this, "Error", "Select a course!");
        return;
    }

    StudentListDialog dlg(courses[row], studentList, this);
    dlg.exec();
}