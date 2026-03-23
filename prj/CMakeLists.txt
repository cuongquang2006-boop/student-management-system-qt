cmake_minimum_required(VERSION 3.16)

project(DemoUI_quiz VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    main_slots.cpp
    mainwindow.cpp
    LoginWindow.cpp
    DetailStudentInformation.cpp
    StudentManagerWidget.cpp
    StudentManagerWidget_slot.cpp
    CoursesManagerWidget.cpp
    StudentListDialog.cpp
    ScoreManagerWidget.cpp
    DashboardWidget.cpp
    ScheduleDialog.cpp
)

set(HEADERS
    mainwindow.h
    LoginWindow.h
    DetailStudentInformation.h
    StudentManagerWidget.h
    Student.h
    User.h
    CoursesManagerWidget.h
    Course.h
    StudentListDialog.h
    Score.h
    ScoreManagerWidget.h
    DashboardWidget.h
    ScheduleDialog.h
)

qt_add_executable(DemoUI_quiz
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(DemoUI_quiz PRIVATE Qt6::Widgets)
