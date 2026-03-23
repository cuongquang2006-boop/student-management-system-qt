#include "StudentListDialog.h"

#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>

StudentListDialog::StudentListDialog(const Course& course,
    const QVector<Student>& students,
    QWidget* parent)
    : QDialog(parent), m_course(course), m_students(students)
{
    setWindowTitle("Students in Course");
    resize(500, 400);

    QVBoxLayout* layout = new QVBoxLayout(this);

    table = new QTableWidget;
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({ "ID", "Name", "Action" });
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(table);

    loadData();
}

void StudentListDialog::loadData()
{
    int row = 0;

    for (const QString& sid : m_course.studentIds)
    {
        for (const Student& s : m_students)
        {
            if (s.id == sid)
            {
                table->insertRow(row);

                table->setItem(row, 0, new QTableWidgetItem(s.id));
                table->setItem(row, 1, new QTableWidgetItem(s.name));

                QPushButton* btn = new QPushButton("View");

                connect(btn, &QPushButton::clicked,
                    this, &StudentListDialog::onViewStudent);

                table->setCellWidget(row, 2, btn);

                row++;
            }
        }
    }
}

void StudentListDialog::onViewStudent()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int row = table->indexAt(btn->pos()).row();
    if (row < 0) return;

    QString id = table->item(row, 0)->text();

    for (const Student& s : m_students)
    {
        if (s.id == id)
        {
            QMessageBox::information(
                this,
                "Student Info",
                "ID: " + s.id +
                "\nName: " + s.name +
                "\nAge: " + QString::number(s.age) +
                "\nGender: " + s.gender +
                "\nMajor: " + s.major +
                "\nClass: " + s.className
            );
            return;
        }
    }
}