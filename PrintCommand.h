//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_PRINTCOMMAND_H
#define FLIGHTSIMULATOR_PRINTCOMMAND_H


#include "Command.h"

class PrintCommand : public Command {
    string str;
public:
    PrintCommand(const string &str);

    int execute(int index, vector<string> &lexer);

};


#endif //FLIGHTSIMULATOR_PRINTCOMMAND_H
