//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_IFCOMMAND_H
#define FLIGHTSIMULATOR_IFCOMMAND_H


#include "ConditionParserCommand.h"

class IfCommand : public ConditionParserCommand {
public:
    IfCommand(const string &leftStr, const string &rightStr);

    int execute(int index, vector<string> &lexer);
};


#endif //FLIGHTSIMULATOR_IFCOMMAND_H
