//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_ASSIGNMENTCOMMAND_H
#define FLIGHTSIMULATOR_ASSIGNMENTCOMMAND_H

#include "Command.h"

using namespace std;

class AssignmentCommand : public Command {
    string _var;
public:
    AssignmentCommand(const string &var);

    int execute(int index, vector<string> &lexer);
};


#endif //FLIGHTSIMULATOR_ASSIGNMENTCOMMAND_H
