//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_FUNCCOMMAND_H
#define FLIGHTSIMULATOR_FUNCCOMMAND_H


#include "Command.h"

class FuncCommand : public Command {
    string var;
    double _val;
    int _startIndex;
    int _endIndex;
public:
    FuncCommand(const string &variable);

    int execute(int index, vector<string> &lexer);

    int getStartIndex() const;

    int getEndIndex() const;

    void setStartIndex(int startIndex);

    void setEndIndex(int endIndex);

    void setVal(double val);

    const string &getVar() const;

};


#endif //FLIGHTSIMULATOR_FUNCCOMMAND_H
