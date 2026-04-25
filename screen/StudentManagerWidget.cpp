#include "StudentManagerWidget.h"
#include "DetailStudentInformation.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QTableWidgetItem>

#include <QFile>
#include <QFileDialog>

#include <QJsonDocument>
#include <QJsonArray>

#include <QMenuBar>
#include <QToolBar>

#include <QPropertyAnimation>
#include <qwidgetaction.h>

#include <QCloseEvent> 


StudentManagerWidget::StudentManagerWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupMenu();
    setupToolbar();
}

void StudentManagerWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("Search by name...");

    connect(searchEdit,&QLineEdit::textChanged,
            this,&StudentManagerWidget::searchStudent);

    mainLayout->addWidget(new QLabel("Search:"));
    mainLayout->addWidget(searchEdit);

    table = new QTableWidget;

    table->setColumnCount(7);
    table->setHorizontalHeaderLabels(
        {"ID","Name","Age","Gender","Major","Class","Action"});

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(table,&QTableWidget::cellClicked,
            this,&StudentManagerWidget::loadToForm);

    mainLayout->addWidget(table);

    idEdit = new QLineEdit;
    idEdit->setMaxLength(9);
    QLabel* label = new QLabel;
    idEdit->setPlaceholderText("Remaining 9 characters");

    connect(idEdit, &QLineEdit::textChanged, this, [=]() {
        int maxLen = idEdit->maxLength();
        int currentLen = idEdit->text().length();
        int remain = maxLen - currentLen;

        label->setText("Remaining " + QString::number(remain) + " characters");
        });

    nameEdit = new QLineEdit;
    nameEdit->setMaxLength(50);
    nameEdit->setPlaceholderText("Enter up to 50 characters");

    ageSpin = new QSpinBox;
    ageSpin->setRange(18,100);

    genderBox = new QComboBox;
    genderBox->addItems({"<<None>>","Male","Female","Other"});

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

    majorBox = new QComboBox;
    majorBox->addItems({
        "<<None>>",
        "Information Technology",
        "Software Engineering",
        "Artificial Intelligence",
        "Data Science",
        "Marketing",
        "Business Administration"
        });

    majorClassMap["Information Technology"] = { "CNTT1","CNTT2","CNTT3","CNTT4"};
    majorClassMap["Software Engineering"]   = { "SE1","SE2","SE3","SE4"};
    majorClassMap["Artificial Intelligence"]= { "AI1","AI2","AI3","AI4"};
    majorClassMap["Data Science"]           = { "DS1","DS2","DS3","DS4"};
    majorClassMap["Marketing"]              = { "Marketing1","Marketing2","Marketing3","Marketing4" };
    majorClassMap["Business Administration"]= { "Business1","Business2","Business3","Business4" };

    QFormLayout *formLayout = new QFormLayout;

    formLayout->addRow("", label);              
    formLayout->addRow("Student ID:",idEdit);
    formLayout->addRow("Name:",nameEdit);
    formLayout->addRow("Age:",ageSpin);
    formLayout->addRow("Gender:",genderBox);
    formLayout->addRow("Major:",majorBox);
    formLayout->addRow("Class:",classBox);

    addBtn      = new QPushButton("Add Student");
    updateBtn   = new QPushButton("Update Student");
    deleteBtn   = new QPushButton("Delete Student");
    clearBtn    = new QPushButton("Clear Form");

    QPushButton *backBtn = new QPushButton("← Back");

    connect(backBtn,&QPushButton::clicked,[this]()
            {
                QPropertyAnimation *anim = new QPropertyAnimation(this,"pos");

                anim->setDuration(220); 
                anim->setStartValue(pos());
                anim->setEndValue(QPoint(width(), pos().y())); 

                connect(anim,&QPropertyAnimation::finished,this,[this]()
                        {
                            emit backRequested();
                            move(0,pos().y()); 
                        });

                anim->start(QAbstractAnimation::DeleteWhenStopped);
            });

    connect(addBtn, &QPushButton::clicked,
        this, &StudentManagerWidget::addStudent);

    connect(updateBtn, &QPushButton::clicked,
        this, &StudentManagerWidget::updateStudent);

    connect(deleteBtn, &QPushButton::clicked,
        this, &StudentManagerWidget::deleteStudent);

    connect(clearBtn, &QPushButton::clicked,
        this, &StudentManagerWidget::clearForm);

    connect(majorBox, &QComboBox::currentTextChanged,
        this, &StudentManagerWidget::updateClassByMajor);


    QHBoxLayout *btnLayout = new QHBoxLayout;

    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(clearBtn);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    bottomLayout->addStretch();      
    bottomLayout->addWidget(backBtn);

    mainLayout->addLayout(bottomLayout);
}

bool StudentManagerWidget::isModified() const
{
    return modified;
}

void StudentManagerWidget::setModified(bool value)
{
    modified = value;
}

void StudentManagerWidget::setStudents(const QVector<Student>& list)
{
    students = list;
    refreshTable();
}

const QVector<Student>& StudentManagerWidget::getStudents() const
{
    return students;
}

void StudentManagerWidget::setupMenu()
{
    QMenuBar* menuBar = new QMenuBar;

    QMenu* fileMenu = menuBar->addMenu("File");

    QAction* openAct = new QAction("Open", this);
    QAction* saveAct = new QAction("Save", this);

    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);

    connect(openAct, &QAction::triggered,
        this, &StudentManagerWidget::loadFromFile);

    connect(saveAct, &QAction::triggered,
        this, &StudentManagerWidget::saveToFile);

    QLabel* noteLabel = new QLabel("  Only saves student list (no courses)");
    noteLabel->setStyleSheet("color: gray; font-size: 12px;");

    QWidgetAction* labelAction = new QWidgetAction(this);
    labelAction->setDefaultWidget(noteLabel);

    menuBar->addAction(labelAction);

    layout()->setMenuBar(menuBar);
}

void StudentManagerWidget::setupToolbar()
{
    QToolBar *toolbar = new QToolBar;

    QAction *addAct = toolbar->addAction("Add");
    QAction *updateAct = toolbar->addAction("Update");
    QAction *deleteAct = toolbar->addAction("Delete");
    QAction *saveAct = toolbar->addAction("Save");

    connect(addAct, &QAction::triggered,
            this, &StudentManagerWidget::addStudent);

    connect(updateAct, &QAction::triggered,
            this, &StudentManagerWidget::updateStudent);

    connect(deleteAct, &QAction::triggered,
            this, &StudentManagerWidget::deleteStudent);

    connect(saveAct, &QAction::triggered,
            this, &StudentManagerWidget::saveToFile);

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    mainLayout->insertWidget(0, toolbar);
}

void StudentManagerWidget::loadToForm(int row, int)
{
    const Student& s = students[row];

    idEdit->setText(s.id);
    nameEdit->setText(s.name);
    ageSpin->setValue(s.age);
    genderBox->setCurrentText(s.gender);
    majorBox->setCurrentText(s.major);
    classBox->setCurrentText(s.className);
}

void StudentManagerWidget::clearForm()
{
    idEdit->clear();
    nameEdit->clear();
    ageSpin->setValue(18);
    genderBox->setCurrentText("<<None>>");
    majorBox->setCurrentText("<<None>>");
    classBox->setCurrentText("<<None>>");
}

void StudentManagerWidget::refreshTable()
{
    table->setRowCount(students.size());

    for(int i=0;i<students.size();++i)
    {
        const Student &s = students[i];

        table->setItem(i,0,new QTableWidgetItem(s.id));
        table->setItem(i,1,new QTableWidgetItem(s.name));
        table->setItem(i,2,new QTableWidgetItem(QString::number(s.age)));
        table->setItem(i,3,new QTableWidgetItem(s.gender));
        table->setItem(i,4,new QTableWidgetItem(s.major));
        table->setItem(i,5,new QTableWidgetItem(s.className));

        QPushButton *viewBtn = new QPushButton("View");

        connect(viewBtn,&QPushButton::clicked,[this,i]()
                {
                    openDetailInformationStudent(i);
                });

        table->setCellWidget(i,6,viewBtn);
    }
}

void StudentManagerWidget::openDetailInformationStudent(int i)
{
    DetailStudentInformation dialog(students[i], this);

    if (dialog.exec() == QDialog::Accepted)
    {
        students[i] = dialog.getStudent();
        refreshTable();
    }
}
