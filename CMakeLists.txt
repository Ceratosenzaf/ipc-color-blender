cmake_minimum_required(VERSION 3.16)

project(main CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(main src/main.cpp)

target_include_directories(main PRIVATE src)

target_link_libraries(main PRIVATE glfw OpenGL::GL)