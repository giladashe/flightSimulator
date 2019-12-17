//
// Created by shaiyis on 12/16/19.
//

#ifndef FLIGHTSIMULATOR_VARIABLES_H
#define FLIGHTSIMULATOR_VARIABLES_H


#include <unordered_map>
#include "VarData.h"
#include "Expression.h"
#include "ex1.h"
#include "Command.h"

// singleton Variables  so all the program has access

class Variables {
    Variables(); // private constructor
    unordered_map<string, VarData*> progMap;
    unordered_map<string, VarData*> simMap;
    unordered_map<string,Command*> commandMap;
    static Variables* instance;
    Interpreter* interpreter;
public:
    static Variables* getInstance();
    unordered_map<string, VarData*> getProgMap();
    unordered_map<string, VarData*> getSimMap();
    unordered_map<string,Command*> &getCommandMap();
    Interpreter* getInterpreter();
    // searches for variables in the curent line and update in "setVariables"
    // of Interpreter
    void updateVariables (int index, vector<string> &lexer);
    int calculate (string s);
};

//TODO nullptr
Variables* Variables::instance = 0;



#endif //FLIGHTSIMULATOR_VARIABLES_H
