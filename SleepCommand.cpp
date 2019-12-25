//
// Created by giladasher on 25/12/2019.
//

#include <chrono>
#include <thread>
#include "SleepCommand.h"

SleepCommand::SleepCommand(const string &str) : str(str) {}

int SleepCommand::execute(int index, vector <string> &lexer) {
    std::this_thread::sleep_for(std::chrono::milliseconds(stol(lexer[index + 1])));
    return 3;
}