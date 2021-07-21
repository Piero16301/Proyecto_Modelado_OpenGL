cmake_minimum_required(VERSION 3.19)
project(3D_Model_Loader)

set(CMAKE_CXX_STANDARD 20)

add_executable(3D_Model_Loader main.cpp Modelo.h Material.h Cara.h)

target_link_libraries(3D_Model_Loader -lopengl32 -lglu32 -lfreeglut)