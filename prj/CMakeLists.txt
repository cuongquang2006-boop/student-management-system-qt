cmake_minimum_required(VERSION 3.16)

project(DemoUI_quiz VERSION 1.0 LANGUAGES CXX)

# --- C++ Standard ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Qt6 AUTOMOC, AUTOUIC, AUTORCC ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- Tìm Qt6 Widgets ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Sources ---
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
)

# --- Headers ---
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
)

# --- Tạo executable ---
qt_add_executable(DemoUI_quiz
    ${SOURCES}
    ${HEADERS}
)

# --- Link Qt6 Widgets ---
target_link_libraries(DemoUI_quiz PRIVATE Qt6::Widgets)

# --- (Optional) Build output folder) ---
# set_target_properties(DemoUI_quiz PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
# )