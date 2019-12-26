//
// Created by shaiyis on 12/14/19.
//

#include <unistd.h>
#include "Parser.h"
#include "Command.h"
#include "Data.h"
#include "MakeFuncCommand.h"

using namespace std;

Parser::Parser(vector<string> &lex) {
    this->lexer = lex;
}

Parser::~Parser() {
}

using namespace std;
void Parser::parse(){
    int index = 0;
    while (index < this->lexer.size()) {
        // sleep process for 1s
        sleep(1);
        Data::getInstance()->comMapMutex.lock();
        auto commandMap = Data::getInstance()->getCommandMap();
        auto it = commandMap.find(this->lexer[index]);
        if (it != commandMap.end()){
            Command *command = it->second;
            index += command->execute(index, this->lexer);
        } else if (lexer[index + 1] == "("){
            //function definition
            Command *makeFuncCommand = new MakeFuncCommand();
            index += makeFuncCommand->execute(index, this->lexer);
        } else {
            // assignmentCommand
            Command *assignmentCommand = Data::getInstance()->getCommandMap().find("assign")->second;
            index += assignmentCommand->execute(index, this->lexer);
        }
        Data::getInstance()->comMapMutex.unlock();

    }
    Data::getInstance()->setStop(true);
}
//