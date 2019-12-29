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
    string _leftStr;
    string _rightStr;
public:
    ConditionParserCommand(const string &leftStr, const string &rightStr);

    int execute(int index, vector<string> &lexer)=0;

    // checks if the condition is true or false
    bool checkCondition(int index, vector<string> &lexer);

    // calculates steps to the first command
    int stepsToFirstCommand(int index, vector<string> &lexer);

    // calculates steps out of the scope
    int stepOutOfTheScope(int index, vector<string> &lexer);
};

#endif //FLIGHTSIMULATOR_CONDITIONPARSERCOMMAND_H
