//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_CONDITIONPARSERCOMMAND_H
#define FLIGHTSIMULATOR_CONDITIONPARSERCOMMAND_H

#include "Command.h"
#include <list>

using namespace std;

// different from Condition (extends BinaryOperator)
class ConditionParserCommand : public Command {
protected:
    string leftStr;
    string rightStr;
    list<Command> commandList;
public:
    ConditionParserCommand(const string &leftStr, const string &rightStr);

    int execute(int index, vector<string> &lexer);

    const list<Command> &getCommandList() const;

    double checkCondition(int index, vector<string> &lexer);

    int stepsToFirstCommand(int index, vector<string> &lexer);

    int stepOutOfTheScope(int index, vector<string> &lexer);
};

#endif //FLIGHTSIMULATOR_CONDITIONPARSERCOMMAND_H
