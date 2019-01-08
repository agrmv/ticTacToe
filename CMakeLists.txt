cmake_minimum_required(VERSION 3.11)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp game.hpp glut.hpp)

INCLUDE(FindPkgConfig)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})