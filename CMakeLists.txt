cmake_minimum_required(VERSION 3.19)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler main.cpp word.cpp word.h lexicalAnalysis.cpp lexicalAnalysis.h defination.cpp)