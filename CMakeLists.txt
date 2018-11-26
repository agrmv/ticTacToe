cmake_minimum_required(VERSION 3.11)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp game.hpp glut.hpp)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL_ttf REQUIRED)
#PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})