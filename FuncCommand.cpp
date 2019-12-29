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
    if((lexer[index + 1]).empty()){
        cerr<<"something happend in func.."<<endl;
        exit(1);
    }
    Data::updateVariablesFromStr(lexer[index+1]);
    Expression* e = data->getInterpreter()->interpret(lexer[index+1]);
    double value = e->calculate();
    data->setProgMap(this->var, new VarData(value, "", "", 0));

    int i = this->_startIndex;
    int j = this->_endIndex;
    int jump = 0;
    while (i < j) {
        Command* command1 = (Command*)data->getCommandMap(lexer[i]);
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

    data->removeFromProgMap(this->var);

    return jump+1;
}

void FuncCommand::setStartIndex(int startIndex) {
    this->_startIndex = startIndex;
}

void FuncCommand::setEndIndex(int endIndex) {
    this->_endIndex = endIndex;
}
