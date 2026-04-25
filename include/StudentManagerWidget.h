#ifndef STUDENTMANAGERWIDGET_H
#define STUDENTMANAGERWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QMap>
#include "Student.h"

class StudentManagerWidget : public QWidget
{
    Q_OBJECT

public:
    StudentManagerWidget(QWidget *parent = nullptr);

signals:
    void backRequested();   

private:

    bool modified = false;
    QVector<Student> students;
    QMap<QString, QStringList> majorClassMap;
    QTableWidget* table;

    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QSpinBox* ageSpin;
    QComboBox* genderBox;
    QComboBox   * majorBox;
    QComboBox* classBox;
    QLineEdit* searchEdit;

    QPushButton* addBtn;
    QPushButton* updateBtn;
    QPushButton* deleteBtn;
    QPushButton* clearBtn;

    void setupUI();
    void refreshTable();
    void clearForm();
    void setupToolbar();
    void setupMenu();
    void openDetailInformationStudent(int i);

public:
    bool isModified() const;
    void setModified(bool value);
    void setStudents(const QVector<Student>& list);
    const QVector<Student>& getStudents() const;

signals:
    void dataChanged();

private slots:
    void addStudent();
    void updateStudent();
    void deleteStudent();
    void loadToForm(int row, int col);
    void searchStudent(const QString& text);
    void updateClassByMajor(const QString& major);
    void saveToFile();
    void loadFromFile();
};

#endif
