//
// Created by shaiyis on 12/14/19.
//
using namespace std;

#include "Parser.h"
#include "Command.h"
#include "Variables.h"
#include "Command.h"


Parser::Parser(vector<string> lex) {
    this->lexer = lex;
}

Parser::~Parser() {
}

void Parser::parse(){
    int index = 0;
    while (index < this->lexer.size()) {
        if (Variables::m.try_lock()) {
            auto it = Variables::getInstance()->getCommandMap().find(this->lexer[index]);
            if (it != Variables::getInstance()->getCommandMap().end()) {
                Command *command = it->second;
                index += command->execute(index, this->lexer);
            } else if (lexer[index + 1] == "(") {
                //function definition
                Command *makeFuncCommand = new MakeFuncCommand();
                index += makeFuncCommand->execute(index, this->lexer);
                //todo command->delete
            } else {
                // assignmentCommand
                Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
                index += assignmentCommand->execute(index, this->lexer);
            }
            Variables::m.unlock();
        }
    }
    if (Variables::m.try_lock()){
        Variables::getInstance()->setStop(true);
        Variables::m.unlock();
    }
    //  Variables::getInstance()->getServerThread()->detach();
    //Variables::getInstance()->getClientThread()->detach();
}
