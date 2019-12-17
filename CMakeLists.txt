cmake_minimum_required(VERSION 3.15)
project(flightSimulator)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(flightSimulator main.cpp Lexer.cpp VarData.cpp VarData.h Parser.cpp Parser.h Variables.cpp
        Variables.h ex1.h ex1.cpp Expression.h Expression.cpp Command.cpp Command.h)