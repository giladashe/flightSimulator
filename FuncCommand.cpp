//
// Created by giladasher on 25/12/2019.
//

#include <iostream>
#include "FuncCommand.h"
#include "Data.h"

FuncCommand::FuncCommand(const string &variable) : var(variable) {}

int FuncCommand::execute(int index, vector <string> &lexer) {
    //insert local variable to the map and at the end erase it
    auto data = Data::getInstance();
    if((lexer[index + 2]).empty()){
        cerr<<"something happend in func.."<<endl;
        exit(1);
    }
    data->setProgMap(this->var, new VarData(stod(lexer[index + 2]), "", "", 0));
    //this->setVal(stod(lexer[index + 2]));
    //todo complex expression

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


    return jump + 1;
}

int FuncCommand::getStartIndex() const {
    return _startIndex;
}

int FuncCommand::getEndIndex() const {
    return _endIndex;
}

void FuncCommand::setStartIndex(int startIndex) {
    this->_startIndex = startIndex;
}

void FuncCommand::setEndIndex(int endIndex) {
    this->_endIndex = endIndex;
}

const string &FuncCommand::getVar() const {
    return this->var;
}

void FuncCommand::setVal(double val) {
    this->_val = val;
}