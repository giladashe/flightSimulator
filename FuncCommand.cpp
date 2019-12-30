//
// Created by giladasher on 25/12/2019.
//

#include <iostream>
#include "FuncCommand.h"
#include "Data.h"

FuncCommand::FuncCommand(const string &variable) : var(variable),_startIndex(0),_endIndex(0) {}

int FuncCommand::execute(int index, vector <string> &lexer) {
    //insert local variable to the map and at the end erase it
    auto data = Data::getInstance();
    Data::updateVariablesFromStr(lexer[index+1]);
    Expression* e = data->getInterpreter()->interpret(lexer[index+1]);
    double value = e->calculate();
    delete e;
    //put the variable in the program map with the value as was interpreted
    data->setProgMap(this->var, new VarData(value, "", "", 0));

    int i = this->_startIndex;
    int j = this->_endIndex;
    int jump = 0;
    while (i < j) {
        //do all the commands of the functions
        auto* command1 = (Command*)data->getCommandMap(lexer[i]);
        if (command1 != nullptr) {
            i += command1->execute(i, lexer);
        } else {
            // assignmentCommand
            Command *assignmentCommand = (Command*)data->getCommandMap("assign");
            i += assignmentCommand->execute(i, lexer);
        }
    }
    for (jump = 0; lexer[index] != "\n"; jump++) {
        index++;
    }
    //remove the variable from the program map
    data->removeFromProgMap(this->var);

    return jump+1;
}

void FuncCommand::setStartIndex(int startIndex) {
    this->_startIndex = startIndex;
}

void FuncCommand::setEndIndex(int endIndex) {
    this->_endIndex = endIndex;
}
