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
    auto data = Data::getInstance();
    while (index < this->lexer.size()) {
        // sleep process for 1s
        sleep(1);
        Command* command1 = (Command*)data->getCommandMap(this->lexer[index]);
        if (command1 != nullptr){
            index += command1->execute(index, this->lexer);
        } else if (lexer[index + 1] == "("){
            //function definition
            Command *makeFuncCommand = new MakeFuncCommand();
            index += makeFuncCommand->execute(index, this->lexer);
        } else {
            // assignmentCommand
            Command *assignmentCommand = (Command*)data->getCommandMap("assign");
            index += assignmentCommand->execute(index, this->lexer);
        }


    }
    Data::getInstance()->setStop(true);
}
//