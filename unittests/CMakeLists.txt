cmake_minimum_required(VERSION 3.21)

project(qml_catch2_console_test)

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL Quick QuickControls2)

qt_add_executable(test_qml_catch2_console main.cpp)
target_link_libraries(test_qml_catch2_console PUBLIC Catch2::Catch2 qml_catch2_console
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::QuickControls2)
