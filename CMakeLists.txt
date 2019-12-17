cmake_minimum_required(VERSION 3.15)
project(flightSimulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(flightSimulator main.cpp Lexer.cpp VarData.cpp VarData.h Command.cpp Command.h Parser.cpp Parser.h Variables.cpp
        Variables.h ex1.h ex1.cpp Expression.h Expression.cpp)