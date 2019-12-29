//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_FUNCCOMMAND_H
#define FLIGHTSIMULATOR_FUNCCOMMAND_H


#include "Command.h"

class FuncCommand : public Command {
    string var;
    int _startIndex;
    int _endIndex;
public:
    FuncCommand(const string &variable);

    int execute(int index, vector<string> &lexer);

    void setStartIndex(int startIndex);

    void setEndIndex(int endIndex);

};


#endif //FLIGHTSIMULATOR_FUNCCOMMAND_H
