cmake_minimum_required(VERSION 3.15)
project(flightSimulator)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(flightSimulator main.cpp Lexer.cpp VarData.cpp VarData.h Parser.cpp Parser.h Data.cpp
        Data.h ex1.h ex1.cpp Expression.h Command.h AssignmentCommand.cpp AssignmentCommand.h
        ConditionParserCommand.cpp ConditionParserCommand.h ConnectClientCommand.cpp ConnectClientCommand.h
        DefineVarCommand.cpp DefineVarCommand.h FuncCommand.cpp FuncCommand.h IfCommand.cpp IfCommand.h
        LoopCommand.cpp LoopCommand.h MakeFuncCommand.cpp MakeFuncCommand.h OpenDataServerCommand.cpp
        OpenDataServerCommand.h PrintCommand.cpp PrintCommand.h SleepCommand.cpp SleepCommand.h)