//
// Created by giladasher on 25/12/2019.
//

#include "FuncCommand.h"
#include "Data.h"

FuncCommand::FuncCommand(const string &variable) : var(variable) {}

int FuncCommand::execute(int index, vector <string> &lexer) {
    //insert local variable to the map and at the end erase it
    Data::getInstance()->m.lock();
    Data::getInstance()->setProgMap(this->var, new VarData(this->_val, "", "", 0));
    this->setVal(stod(lexer[index + 2]));
    Data::getInstance()->m.unlock();

    int i = this->_startIndex;
    int j = this->_endIndex;
    int jump = 0;
    while (i < j) {
        auto it = Data::getInstance()->getCommandMap().find(lexer[index]);
        if (it != Data::getInstance()->getCommandMap().end()) {
            Command *command = it->second;
            i += command->execute(index, lexer);
        } else {
            // assignmentCommand
            Command *assignmentCommand = Data::getInstance()->getCommandMap().find("assign")->second;
            i += assignmentCommand->execute(index, lexer);
        }
    }
    for (jump = 0; lexer[index] != "\n"; jump++) {
        index++;
    }
    Data::getInstance()->m.lock();
    Data::getInstance()->removeFromProgMap(this->var);
    Data::getInstance()->m.unlock();

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