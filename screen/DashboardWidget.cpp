#include "DashboardWidget.h"
#include "ScheduleDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTableWidgetItem>
#include <algorithm>

DashboardWidget::DashboardWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // ===== Stats =====
    QHBoxLayout* statsLayout = new QHBoxLayout;
    auto createCard = [](QString title, QLabel*& valueLabel) {
        QFrame* card = new QFrame;
        card->setStyleSheet("background:#2d7df6;color:white;padding:15px;border-radius:10px;");
        QVBoxLayout* layout = new QVBoxLayout(card);
        QLabel* lblTitle = new QLabel(title);
        valueLabel = new QLabel("0");
        valueLabel->setStyleSheet("font-size:24px;font-weight:bold;");
        layout->addWidget(lblTitle);
        layout->addWidget(valueLabel);
        return card;
        };

    statsLayout->addWidget(createCard("Students", lblStudentCount));
    statsLayout->addWidget(createCard("Courses", lblCourseCount));
    statsLayout->addWidget(createCard("Scores", lblScoreCount));
    mainLayout->addLayout(statsLayout);

    // ===== Top students table =====
    tableTop = new QTableWidget;
    tableTop->setColumnCount(4);
    tableTop->setHorizontalHeaderLabels({ "Rank","ID","Name","Score" });
    tableTop->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableTop->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(new QLabel("Top Students"));
    mainLayout->addWidget(tableTop);

    // ===== Buttons =====
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnViewSchedule = new QPushButton("View Courses");
    btnSaveSchedule = new QPushButton("Save Courses");
    btnLoadSchedule = new QPushButton("Load Courses");
    btnLayout->addWidget(btnViewSchedule);
    btnLayout->addWidget(btnSaveSchedule);
    btnLayout->addWidget(btnLoadSchedule);
    mainLayout->addLayout(btnLayout);

    // connect
    connect(btnViewSchedule, &QPushButton::clicked, this, [this]() {
        ScheduleDialog dlg(this);
        dlg.setSchedule(m_courses);
        dlg.exec();
        });
    connect(btnSaveSchedule, &QPushButton::clicked, this, &DashboardWidget::saveCourses);
    connect(btnLoadSchedule, &QPushButton::clicked, this, &DashboardWidget::loadCourses);
}

void DashboardWidget::setData(const QVector<Student>& students,
    const QVector<Course>& courses,
    const QVector<Score>& scores)
{
    m_students = students;
    m_courses = courses;
    m_scores = scores;
    refresh();
}

void DashboardWidget::refresh()
{
    updateStats();
    updateTopStudents();
}

void DashboardWidget::updateStats()
{
    lblStudentCount->setText(QString::number(m_students.size()));
    lblCourseCount->setText(QString::number(m_courses.size()));
    lblScoreCount->setText(QString::number(m_scores.size()));
}

void DashboardWidget::updateTopStudents()
{
    QVector<Score> sorted = m_scores;
    std::sort(sorted.begin(), sorted.end(), [](const Score& a, const Score& b) {
        return a.total > b.total;
        });

    int topN = std::min<int>(5, sorted.size());
    tableTop->setRowCount(topN);

    for (int i = 0; i < topN; i++) {
        const Score& s = sorted[i];
        QString name;
        for (const Student& st : m_students) {
            if (st.id == s.studentId) { name = st.name; break; }
        }

        tableTop->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        tableTop->setItem(i, 1, new QTableWidgetItem(s.studentId));
        tableTop->setItem(i, 2, new QTableWidgetItem(name));
        tableTop->setItem(i, 3, new QTableWidgetItem(QString::number(s.total, 'f', 2)));
    }
}

// ===== Save / Load =====
void DashboardWidget::saveCourses()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Courses", "", "*.json");
    if (fileName.isEmpty()) return;

    QJsonArray arr;
    for (const Course& c : m_courses)
        arr.append(c.toJson());

    QFile f(fileName);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QJsonDocument(arr).toJson());
        f.close();
    }
}

void DashboardWidget::loadCourses()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Courses", "", "*.json");
    if (fileName.isEmpty()) return;

    QFile f(fileName);
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray data = f.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray arr = doc.array();
        m_courses.clear();
        for (const QJsonValue& v : arr)
            m_courses.append(Course::fromJson(v.toObject()));
        f.close();
        refresh();
    }
}