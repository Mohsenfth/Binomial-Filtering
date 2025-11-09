cmake_minimum_required(VERSION 3.26)
project(ubung3)

find_package( OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

add_executable(ubung3 main.cpp)
target_link_libraries(ubung3 ${OpenCV_LIBS})
set(CMAKE_CXX_STANDARD 17)

