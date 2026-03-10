#include "MainWindow.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUI();        
    setupMenu();      
    setupToolbar();   

    statusBar()->showMessage("READY!");
    resize(900, 600);                  
}

void MainWindow::setupUI()
{
    QWidget *central = new QWidget;  
    setCentralWidget(central);     

    QVBoxLayout *mainLayout = new QVBoxLayout; 

    searchEdit = new QLineEdit; 
    searchEdit->setPlaceholderText("Search by name..."); 

    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::searchStudent);
    mainLayout->addWidget(new QLabel("Search:")); 
    mainLayout->addWidget(searchEdit);            

    table = new QTableWidget;
    table->setColumnCount(5); 
    table->setHorizontalHeaderLabels({"ID", "Name", "Age", "Gender", "Major"});

    table->setSelectionBehavior(QAbstractItemView::SelectRows); 
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(table, &QTableWidget::cellClicked, this, &MainWindow::loadToForm);
    mainLayout->addWidget(table);

    idEdit = new QLineEdit;
    nameEdit = new QLineEdit;

    ageSpin = new QSpinBox;   
    ageSpin->setRange(18, 100); 

    genderBox = new QComboBox;
    genderBox->addItems({"<<None>>","Male", "Female", "Other" });

    majorEdit = new QLineEdit;      

    QFormLayout *formLayout = new QFormLayout; 
    formLayout->addRow("Student ID:", idEdit);   
    formLayout->addRow("Name:", nameEdit);       
    formLayout->addRow("Age:", ageSpin);         
    formLayout->addRow("Gender:", genderBox);
    formLayout->addRow("Major:", majorEdit);

    addBtn = new QPushButton("Add Student");        
    updateBtn = new QPushButton("Update Student");  
    deleteBtn = new QPushButton("Delete Student");  
    clearBtn = new QPushButton("Clear Form");       

    connect(addBtn, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::updateStudent);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::deleteStudent);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::clearForm);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(updateBtn);
    btnLayout->addWidget(deleteBtn);
    btnLayout->addWidget(clearBtn);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    central->setLayout(mainLayout);

    //thanh cuon
    QScrollArea *scroll = new QScrollArea();

    scroll->setWidget(central);
    scroll->setWidgetResizable(true);

    setCentralWidget(scroll);
}


void MainWindow::setupMenu() 
{   
    QMenu *fileMenu = menuBar()->addMenu("File");

    QAction *openAct = fileMenu->addAction("Open");
    QAction *saveAct = fileMenu->addAction("Save");

    connect(openAct, &QAction::triggered, this, &MainWindow::loadFromFile);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveToFile);

    menuBar()->addMenu("Edit");
    menuBar()->addMenu("Help");
}

void MainWindow::setupToolbar()
{
    QToolBar *tb = addToolBar("Toolbar");

    QAction *addAct = tb->addAction("Add");
    QAction *editAct = tb->addAction("Edit");
    QAction *delAct = tb->addAction("Delete");
    QAction *saveAct = tb->addAction("Save");

    connect(addAct, &QAction::triggered, this, &MainWindow::addStudent);
    connect(editAct, &QAction::triggered, this, &MainWindow::updateStudent);
    connect(delAct, &QAction::triggered, this, &MainWindow::deleteStudent);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveToFile);
}

void MainWindow::loadToForm(int row, int)  
{
    const Student& s = students[row];

    idEdit->setText(s.id);
    nameEdit->setText(s.name);
    ageSpin->setValue(s.age);
    genderBox->setCurrentText(s.gender);
    majorEdit->setText(s.major);
}

void MainWindow::clearForm()
{
    idEdit->clear();
    nameEdit->clear();
    ageSpin->setValue(18);
    genderBox->setCurrentText("<<None>>");
    majorEdit->clear();
}

void MainWindow::refreshTable()
{
    table->setRowCount(students.size());

    for (int i = 0; i < students.size(); ++i)
    {
        const Student &s = students[i];

        table->setItem(i, 0, new QTableWidgetItem(s.id));      
        table->setItem(i, 1, new QTableWidgetItem(s.name));     
        table->setItem(i, 2, new QTableWidgetItem(QString::number(s.age)));
        table->setItem(i, 3, new QTableWidgetItem(s.gender));
        table->setItem(i, 4, new QTableWidgetItem(s.major));
    }
}
