cmake_minimum_required(VERSION 3.17)
project(practical_scheduler)

set(CMAKE_CXX_STANDARD 14)

add_executable(practical_scheduler main.cpp quantum.h scheduler.h scheduler.cpp)