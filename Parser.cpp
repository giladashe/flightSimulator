//
// Created by shaiyis on 12/14/19.
//
using namespace std;

#include "Parser.h"
#include "Command.h"


Parser::Parser(vector<string> lex) {
    this->lexer = lex;
    Command* openDataServer = new OpenDataServerCommand("");
    Command* connectClientCommand = new ConnectClientCommand("", "");
    Command* defineVar = new DefineVarCommand("","");
    Command* printCommand = new PrintCommand("");
    Command* sleepCommand = new SleepCommand("");
    Command* ifCommand = new IfCommand("","");
    Command* loopCommand = new LoopCommand("","");
    this->map["openDataServer"] = openDataServer;
    this->map["connectControlClient"] = connectClientCommand;
    this->map["var"] = defineVar;
    this->map["Print"] = printCommand;
    this->map["Sleep"] = sleepCommand;
    this->map["if"] = ifCommand;
    this->map["while"] = loopCommand;
}

Parser::~Parser() {
}
void Parser::parse() {
    int index = 0;
    while (index < this->lexer.size()){
        Command* command = (this->map.find(this->lexer[index]))->second;
        if (command != nullptr){
            index += command->execute(index, this->lexer);
        }

    }
}

