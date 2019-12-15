cmake_minimum_required(VERSION 3.15)
project(flightSimulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(flightSimulator main.cpp Lexer.cpp VarData.cpp VarData.h Command.cpp Command.h Parser.cpp Parser.h cmake-build-debug/ProgramObjMap.cpp cmake-build-debug/ProgramObjMap.h Variables.cpp Variables.h)