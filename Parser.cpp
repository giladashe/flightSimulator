//
// Created by shaiyis on 12/14/19.
//
using namespace std;

#include "Parser.h"
#include "Command.cpp"


Parser::Parser(vector<string> lex) {
    this->lexer = lex;
    string s = " ";
    int z;
    Command* printCommand = new PrintCommand(s);

    this->map["openDataServer"] =
}

Parser::~Parser() {
}
void Parser::parse() {}

