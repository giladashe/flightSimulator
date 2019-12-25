//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_SLEEPCOMMAND_H
#define FLIGHTSIMULATOR_SLEEPCOMMAND_H


#include "Command.h"

class SleepCommand : public Command {
    string str;
public:
    SleepCommand(const string &str);

    int execute(int index, vector<string> &lexer);
};


#endif //FLIGHTSIMULATOR_SLEEPCOMMAND_H
