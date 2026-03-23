#include "ScoreManagerWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>

ScoreManagerWidget::ScoreManagerWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    setupConnections();
}

// UI
void ScoreManagerWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    courseBox = new QComboBox;
    courseBox->setEditable(true);
    courseBox->lineEdit()->setReadOnly(true);
    courseBox->lineEdit()->setPlaceholderText("Select Course");

    studentBox = new QComboBox;
    studentBox->setEditable(true);
    studentBox->lineEdit()->setReadOnly(true);
    studentBox->lineEdit()->setPlaceholderText("Select Student");

    processEdit = new QLineEdit;
    processEdit->setPlaceholderText("Process (0-10, 60%)");

    finalEdit = new QLineEdit;
    finalEdit->setPlaceholderText("Final (0-10, 40%)");

    QPushButton* addBtn = new QPushButton("Add / Update");
    QPushButton* deleteBtn = new QPushButton("Delete");

    addBtn->setObjectName("addBtn");
    deleteBtn->setObjectName("deleteBtn");

    QVBoxLayout* formLayout = new QVBoxLayout;

    // ===== Row 1: Course + Student (dài) =====
    QHBoxLayout* topRow = new QHBoxLayout;
    topRow->addWidget(courseBox, 2);   // stretch lớn hơn
    topRow->addWidget(studentBox, 2);

    // ===== Row 2: Score + Button (ngắn) =====
    QHBoxLayout* bottomRow = new QHBoxLayout;
    bottomRow->addWidget(processEdit, 1);
    bottomRow->addWidget(finalEdit, 1);
    bottomRow->addWidget(addBtn);
    bottomRow->addWidget(deleteBtn);

    // add vào form
    formLayout->addLayout(topRow);
    formLayout->addLayout(bottomRow);

    table = new QTableWidget;
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(
        { "Student", "Course", "Process", "Final", "Total" }
    );

    table->horizontalHeader()->setStretchLastSection(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(table);
}

// CONNECTION
void ScoreManagerWidget::setupConnections()
{
    QPushButton* addBtn = findChild<QPushButton*>("addBtn");
    QPushButton* deleteBtn = findChild<QPushButton*>("deleteBtn");

    connect(addBtn, &QPushButton::clicked,
        this, &ScoreManagerWidget::addOrUpdateScore);

    connect(deleteBtn, &QPushButton::clicked,
        this, &ScoreManagerWidget::deleteScore);

    connect(courseBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &ScoreManagerWidget::onCourseChanged);

    // auto fill score
    connect(studentBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [this](int)
        {
            QString sid = studentBox->currentData().toString();
            QString cid = courseBox->currentData().toString();

            for (const Score& sc : scoreList)
            {
                if (sc.studentId == sid && sc.courseId == cid)
                {
                    processEdit->setText(QString::number(sc.process));
                    finalEdit->setText(QString::number(sc.final));
                    return;
                }
            }

            processEdit->clear();
            finalEdit->clear();
        });
}

// SET DATA
void ScoreManagerWidget::setData(const QVector<Student>& students,
    const QVector<Course>& courses,
    const QVector<Score>& scores)
{
    studentList = students;
    courseList = courses;
    scoreList = scores;

    courseBox->clear();

    for (const Course& c : courseList)
    {
        if (!c.studentIds.isEmpty())
        {
            courseBox->addItem(c.name + " (" + c.id + ")", c.id);
        }
    }

    if (courseBox->count() > 0)
    {
        courseBox->setCurrentIndex(0);
        updateStudentBox();
    }

    refreshTable();
}

// UPDATE STUDENT
void ScoreManagerWidget::onCourseChanged(int)
{
    updateStudentBox();
}

void ScoreManagerWidget::updateStudentBox()
{
    studentBox->clear();

    QString cid = courseBox->currentData().toString();
    if (cid.isEmpty()) return;

    for (const Course& c : courseList)
    {
        if (c.id == cid)
        {
            for (const QString& sid : c.studentIds)
            {
                for (const Student& s : studentList)
                {
                    if (s.id == sid)
                    {
                        studentBox->addItem(
                            s.name + " (" + s.id + ")", s.id
                        );
                        break;
                    }
                }
            }
        }
    }
}

// ADD
void ScoreManagerWidget::addOrUpdateScore()
{
    QString sid = studentBox->currentData().toString();
    QString cid = courseBox->currentData().toString();

    if (cid.isEmpty() || sid.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Select course & student!");
        return;
    }

    double process = processEdit->text().toDouble();
    double final = finalEdit->text().toDouble();

    if (process < 0 || process > 10 || final < 0 || final > 10)
    {
        QMessageBox::warning(this, "Error", "Score must be 0-10!");
        return;
    }

    for (Score& sc : scoreList)
    {
        if (sc.studentId == sid && sc.courseId == cid)
        {
            sc.process = process;
            sc.final = final;
            sc.total = Score::calcTotal(process, final);

            refreshTable();
            updateStudentBox();
            return;
        }
    }

    scoreList.append(Score(sid, cid, process, final));

    refreshTable();
    updateStudentBox();
}

// DELETE
void ScoreManagerWidget::deleteScore()
{
    int row = table->currentRow();
    if (row < 0) return;

    scoreList.removeAt(row);

    refreshTable();
    updateStudentBox();
}

// REFRESH
void ScoreManagerWidget::refreshTable()
{
    table->setRowCount(scoreList.size());

    for (int i = 0; i < scoreList.size(); ++i)
    {
        const Score& sc = scoreList[i];

        QString studentName, courseName;

        for (const Student& s : studentList)
            if (s.id == sc.studentId)
                studentName = s.name;

        for (const Course& c : courseList)
            if (c.id == sc.courseId)
                courseName = c.name;

        table->setItem(i, 0, new QTableWidgetItem(studentName));
        table->setItem(i, 1, new QTableWidgetItem(courseName));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(sc.process)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(sc.final)));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(sc.total)));
    }
}

// FIX LỖI LINKER
const QVector<Score>& ScoreManagerWidget::getScores() const
{
    return scoreList;
}