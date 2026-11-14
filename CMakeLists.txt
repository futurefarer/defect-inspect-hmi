cmake_minimum_required(VERSION 3.16)

project(hello_vision LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到系统里安装的 OpenCV（Ubuntu 下由 libopencv-dev 提供）
find_package(OpenCV REQUIRED)

add_executable(hello_vision src/main.cpp)

target_include_directories(hello_vision PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(hello_vision PRIVATE ${OpenCV_LIBS})

if(NOT MSVC)
    target_compile_options(hello_vision PRIVATE -Wall -Wextra)
endif()
