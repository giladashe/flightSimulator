//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_PRINTCOMMAND_H
#define FLIGHTSIMULATOR_PRINTCOMMAND_H


#include "Command.h"

class PrintCommand : public Command {
    string _str;
public:
    PrintCommand(const string &str);

    //prints an expression or string to the console
    int execute(int index, vector<string> &lexer);

};


#endif //FLIGHTSIMULATOR_PRINTCOMMAND_H
