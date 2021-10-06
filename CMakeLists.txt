cmake_minimum_required(VERSION 3.19)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler main.cpp class/word.cpp include/word.h lexicalAnalysis/lexicalAnalysis.cpp include/lexicalAnalysis.h grammaAnalysis/grammaAnalysis.cpp include/grammaAnalysis.h settings/defination.h)