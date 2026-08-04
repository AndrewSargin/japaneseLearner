cmake_minimum_required(VERSION 3.20)
project (japanese-learner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(SQLite3 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets Quick QuickWidgets)

add_executable(japanese-learner
    src/main.cpp
    src/engine/subtitle_parser.cpp
    src/engine/dictionary.cpp
    src/engine/engine.cpp
    src/engine/tokenizer.cpp
    src/ui/mpv_widget.cpp
    src/ui/main_window.cpp
    src/ui/subtitle_widget.cpp
    src/ui/mpv_renderer.cpp
    src/ui/engine_qt.cpp
)

target_include_directories(japanese-learner PRIVATE
    /usr/include
    src/ui
    src/engine
)
target_link_libraries(japanese-learner PRIVATE
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Quick
    Qt6::QuickWidgets
    SQLite3::SQLite3
    mecab
    mpv
)

qt_add_qml_module(japanese-learner
    URI JapaneseLearner
    QML_FILES src/ui/main.qml
)
