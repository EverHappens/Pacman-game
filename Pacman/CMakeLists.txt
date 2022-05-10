cmake_minimum_required(VERSION 3.20)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)

add_executable(Pacman main.cpp game.h pacman.h game.cpp player.h player.cpp pacman.cpp global.hpp mapCollision.cpp mapCollision.h cell.h)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(Pacman sfml-system sfml-window sfml-graphics sfml-audio sfml-network)