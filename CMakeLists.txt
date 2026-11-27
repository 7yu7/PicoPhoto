cmake_minimum_required(VERSION 3.16)

project(PicoPhoto VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(PicoPhoto
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/CanvasWidget.h
    src/CanvasWidget.cpp
    src/Document.h
    src/Document.cpp
    src/Brush.h
)

target_link_libraries(PicoPhoto PRIVATE Qt6::Widgets)
