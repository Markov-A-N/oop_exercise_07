cmake_minimum_required (VERSION 3.2)

project(oop_exercise_07)

set(CMAKE_CXX_STANDARD 17)

add_executable(task text.cpp command.cpp document.cpp editor.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
