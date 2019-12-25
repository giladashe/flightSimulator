//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_MAKEFUNCCOMMAND_H
#define FLIGHTSIMULATOR_MAKEFUNCCOMMAND_H


#include "Command.h"

class MakeFuncCommand : public Command {

public:
    int execute(int index, vector<string> &lexer);
};


#endif //FLIGHTSIMULATOR_MAKEFUNCCOMMAND_H
