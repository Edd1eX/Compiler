cmake_minimum_required(VERSION 3.19)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler main.cpp class/word.cpp include/word.h lexicalAnalysis/lexicalAnalysis.cpp include/lexicalAnalysis.h settings/defination.h class/syntax.cpp include/syntax.h syntaxAnalysis/syntaxAnalysis.cpp include/syntaxAnalysis.h)