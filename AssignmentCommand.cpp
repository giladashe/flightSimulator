//
// Created by giladasher on 25/12/2019.
//

#include "AssignmentCommand.h"
#include "Data.h"
#include "Lexer.h"

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
    data->setValueProgMap(varAndVal[0], value);


    if (data->getBindFromProgMap(strToAssign) == 1) {
        string simStr = data->getSimFromProgMap(strToAssign);
        if (strToAssign != "blc") {
            data->setValueSimMap(simStr, value);
        }
        //put command in the queue so the client would send it to the simulator
        string message = "set " + simStr.substr(1, simStr.size() - 1) + " " + to_string(value) + " \r\n";
        data->queueMutex.lock();
        data->commandsQueue.push(message);
        data->queueMutex.unlock();
    }

    // calculate steps to the next command in "lexer"
    int toJump = 0;
    int j = index;
    while (lexer[j] != "\n") {
        j++;
        toJump++;
    }
    return (toJump + 1);
}