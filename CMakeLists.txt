cmake_minimum_required(VERSION 3.24)
project(CSE340_Project3_Provided_Code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CSE340_Project3_Provided_Code
        compiler.cc
        compiler.h
        inputbuf.cc
        inputbuf.h
        lexer.cc
        lexer.h
        parser.cpp)
