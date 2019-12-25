//
// Created by giladasher on 25/12/2019.
//

#include "AssignmentCommand.h"
#include "Data.h"
#include "Lexer.h"
#include <algorithm>

using namespace std;

AssignmentCommand::AssignmentCommand(const string &var) : _var(var) {
}

int AssignmentCommand::execute(int index, vector<string> &lexer) {
    //
    string strToAssign = lexer[index];

    auto data = Data::getInstance();
    //update setVariables on Interpreter
    data->updateVariables(index, lexer);

    // string to calculate
    int i = index;
    string strToCalculate;
    while (lexer[i] != "\n") {
        strToCalculate += lexer[i];
        i++;
    }
    vector<string> varAndVal = Lexer::splitByDelimiter(strToCalculate, "=");
    double value = data->calculate(varAndVal[1]);

    // assign the value as was calculated
    data->m.lock();
    data->setValueProgMap(varAndVal[0], value);

    auto prog_map = data->getProgMap();

    if (prog_map[strToAssign]->getBind() == 1) {
        data->setValueSimMap(prog_map[strToAssign]->getSimStr(), value);
        string simStr = prog_map[strToAssign]->getSimStr();
        string message = "set " + simStr.substr(1, simStr.size() - 1) + " " + to_string(value) + " \r\n";
        data->commandsQueue.push(message);
    }
    data->m.unlock();

    // calculate steps to the next command in "lexer"
    int toJump = 0;
    int j = index;
    while (lexer[j] != "\n") {
        j++;
        toJump++;
    }
    return (toJump + 1);
}