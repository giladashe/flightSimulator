//
// Created by shaiyis on 12/14/19.
//

#include "Parser.h"
#include "Command.h"
#include "Variables.h"


using namespace std;

Parser::Parser(vector<string> lex) {
    this->lexer = lex;
}

Parser::~Parser() {
}

using namespace std;
void Parser::parse(){
    int index = 0;
    while (index < this->lexer.size()) {
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
    }
    Variables::getInstance()->setStop(true);
}
