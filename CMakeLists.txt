cmake_minimum_required(VERSION 3.20)
project (japanese-learner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(SQLite3 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

add_executable(japanese-learner
    src/main.cpp
    src/engine/subtitle_parser.cpp
    src/engine/dictionary.cpp
    src/engine/engine.cpp
    src/engine/tokenizer.cpp
    src/ui/mpv_widget.cpp
    src/ui/main_window.cpp
    src/ui/subtitle_widget.cpp
)

target_include_directories(japanese-learner PRIVATE /usr/include)
target_link_libraries(japanese-learner
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    SQLite3::SQLite3
    mecab
    mpv
)
