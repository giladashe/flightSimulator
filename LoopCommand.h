//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_LOOPCOMMAND_H
#define FLIGHTSIMULATOR_LOOPCOMMAND_H


#include "ConditionParserCommand.h"

class LoopCommand : public ConditionParserCommand {
public:
    LoopCommand(const string &leftStr, const string &rightStr);

    int execute(int index, vector<string> &lexer);
};


#endif //FLIGHTSIMULATOR_LOOPCOMMAND_H
