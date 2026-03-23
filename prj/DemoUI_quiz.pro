QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    DetailStudentInformation.cpp \
    LoginWindow.cpp \
    StudentManagerWidget.cpp \
    DashboardWidget.cpp \
    Course.cpp \
    Score.cpp \
    main.cpp \
    main_slots.cpp \
    mainwindow.cpp

HEADERS += \
    DetailStudentInformation.h \
    LoginWindow.h \
    Student.h \
    StudentManagerWidget.h \
    DashboardWidget.h \
    Course.h \
    Score.h \
    User.h \
    mainwindow.h

# ❌ nếu không dùng thì xóa
# FORMS += mainwindow.ui