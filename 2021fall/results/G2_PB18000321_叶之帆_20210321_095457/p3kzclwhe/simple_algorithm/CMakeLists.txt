cmake_minimum_required(VERSION 3.17)
project(para_scheduler)

set(CMAKE_CXX_STANDARD 14)

add_executable(para_scheduler main.cpp quantum.h scheduler.h scheduler.cpp)