cmake_minimum_required(VERSION 3.20)
project (japanese-learner)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(japanese-learner
    src/main.cpp
    src/engine/subtitle_parser.cpp
    src/engine/dictionary.cpp
)

find_package(SQLite3 REQUIRED)
target_link_libraries(japanese-learner SQLite3::SQLite3)
