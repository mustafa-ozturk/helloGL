cmake_minimum_required(VERSION 3.22)

project(main)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(vendor)

add_executable(main src/main.cpp vendor/stb_image/stb_image.cpp)

find_package ( OpenGL REQUIRED )
target_link_libraries(main OpenGL::GL )

find_package ( GLEW REQUIRED )
target_link_libraries(main GLEW::GLEW )

find_package ( glfw3 REQUIRED )
target_link_libraries(main glfw )