cmake_minimum_required(VERSION 3.26)
project(binomial_filter)

find_package( OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

add_executable(binomial_filter main.cpp)
target_link_libraries(binomial_filter ${OpenCV_LIBS})
set(CMAKE_CXX_STANDARD 17)

