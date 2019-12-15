//
// Created by shaiyis on 12/14/19.
//

#ifndef FLIGHTSIMULATOR_PARSER_H
#define FLIGHTSIMULATOR_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Command.h"
#include "ProgramObjMap.h"

class Parser {
    unordered_map<string,Command*> map;
    ProgramObjMap programObjMap;
    vector<string> lexer;
public:
    Parser(vector<string> lex);
    virtual ~Parser();
    void parse();
};


#endif //FLIGHTSIMULATOR_PARSER_H
