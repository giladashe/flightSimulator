//
// Created by shaiyis on 12/13/19.
//
//
#ifndef FLIGHTSIMULATOR_COMMAND_H
#define FLIGHTSIMULATOR_COMMAND_H

#include <string>
#include <vector>
//#include <thread>
//#include <netinet/in.h>
//#include <algorithm>
//#include "ex1.h"

using namespace std;

class Command {

public:
    virtual ~Command() {}

    virtual int execute(int index, vector<string> &lexer)=0;
};

#endif //FLIGHTSIMULATOR_COMMAND_H