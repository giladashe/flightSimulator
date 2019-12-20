//
// Created by shaiyis on 12/14/19.
//
using namespace std;

#include "Parser.h"
#include "Command.h"
#include "Variables.h"


Parser::Parser(vector<string> lex) {
    this->lexer = lex;
}

Parser::~Parser() {
}

void Parser::parse() {
    int index = 0;
    while (index < this->lexer.size()) {
        auto it = Variables::getInstance()->getCommandMap().find(this->lexer[index]);
        if (it != Variables::getInstance()->getCommandMap().end()) {
            Command *command = it->second;
            index += command->execute(index, this->lexer);
        }
            // assignmentCommand
        else {
            Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
            index += assignmentCommand->execute(index, this->lexer);
        }
    }
    Variables::getInstance()->setStop(true);
  //  Variables::getInstance()->getServerThread()->detach();
    //Variables::getInstance()->getClientThread()->detach();
}

