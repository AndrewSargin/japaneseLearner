cmake_minimum_required(VERSION 3.20)
project (japanese-learner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(SQLite3 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets Quick QuickWidgets Multimedia)
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)

add_executable(japanese-learner
    src/main.cpp
    src/engine/subtitle_parser.cpp
    src/engine/dictionary.cpp
    src/engine/engine.cpp
    src/engine/tokenizer.cpp
    src/qt/engine_qt.cpp
    src/qt/video_player.cpp
    src/qt/qt_multimedia_subtitle_provider.cpp
)

target_include_directories(japanese-learner PRIVATE
    /usr/include
    src/qt
    src/engine
    ${AVFORMAT_INCLUDE_DIRS}
    ${AVCODEC_INCLUDE_DIRS}
    ${AVUTIL_INCLUDE_DIRS}
)
target_link_libraries(japanese-learner PRIVATE
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::Multimedia
    SQLite3::SQLite3
    avformat
    avcodec
    avutil
    mecab
    mpv
)

qt_add_qml_module(japanese-learner
    URI JapaneseLearner
    VERSION 1.0
    
    SOURCES
    src/qt/video_player.h
    src/qt/video_player.cpp

    QML_FILES
        src/ui/main.qml
        src/ui/components/ButtonStyled.qml
        src/ui/components/SubtitleDialog.qml

)
