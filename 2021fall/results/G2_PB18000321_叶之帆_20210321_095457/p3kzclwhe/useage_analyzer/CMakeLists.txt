cmake_minimum_required(VERSION 3.17)
project(program_analyzer)

set(CMAKE_CXX_STANDARD 14)

add_executable(analyzer src/main.cpp src/QASMParser.h src/QASMParser.cpp src/basis.h src/qmachine.h)