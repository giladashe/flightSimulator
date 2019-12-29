//
// Created by giladasher on 25/12/2019.
//

#include <chrono>
#include <thread>
#include "SleepCommand.h"
#include "Expression.h"
#include "Data.h"

SleepCommand::SleepCommand(const string &str) : _str(str) {}

int SleepCommand::execute(int index, vector <string> &lexer) {
    auto data = Data::getInstance();
    //put variables in iterpreter,interpret and calculate the expression
    Data::updateVariablesFromStr(lexer[index + 1]);
    Expression* e = data->getInterpreter()->interpret(lexer[index + 1]);
    double howMuchSleep = e->calculate();
    //sleep for some X milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(stol(to_string(howMuchSleep))));
    return 3;
}