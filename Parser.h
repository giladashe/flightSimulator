//
// Created by shaiyis on 12/14/19.
//

#ifndef FLIGHTSIMULATOR_PARSER_H
#define FLIGHTSIMULATOR_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Command.cpp"

class Parser {
    unordered_map<string,Command> map;
    vector<string> lexer;
public:
    Parser(vector<string> lex);
    virtual ~Parser();
    void parse();
};


#endif //FLIGHTSIMULATOR_PARSER_H