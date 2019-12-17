//
// Created by shaiyis on 12/16/19.
//

#include "Variables.h"
#include <regex>


unordered_map<string, VarData*> Variables::getProgMap(){
    return this->progMap;
}
unordered_map<string, VarData*> Variables::getSimMap(){
    return this->simMap;
}

Variables* Variables::getInstance()
{
    if (instance == 0)
    {
        instance = new Variables();
    }
    return instance;
}

Variables::Variables(){
    this->interpreter = new Interpreter();
    Command* openDataServer = new OpenDataServerCommand("");
    Command* connectClientCommand = new ConnectClientCommand("", "");
    Command* defineVar = new DefineVarCommand("","");
    Command* printCommand = new PrintCommand("");
    Command* sleepCommand = new SleepCommand("");
    Command* ifCommand = new IfCommand("","");
    Command* loopCommand = new LoopCommand("","");
    Command* assignmentCommand = new AssignmentCommand("");
    this->commandMap["openDataServer"] = openDataServer;
    this->commandMap["connectControlClient"] = connectClientCommand;
    this->commandMap["var"] = defineVar;
    this->commandMap["Print"] = printCommand;
    this->commandMap["Sleep"] = sleepCommand;
    this->commandMap["if"] = ifCommand;
    this->commandMap["while"] = loopCommand;
    this->commandMap["assign"] = assignmentCommand;
}

int Variables::calculate(string s){
    return this->interpreter->interpret(s)->calculate();
}
Interpreter* Variables::getInterpreter() {
    return this->interpreter;
}

unordered_map<string, Command*> &Variables::getCommandMap(){
    return commandMap;
}
void Variables::updateVariables(int index, vector<string> &lexer) {
    regex variableRegex ("[a-z|A-Z|_]+[a-z|A-Z|_|0-9]*");
    smatch smatch1;
    int i = index;

    // update the variables values on "setVariables" at Interpreter
    while(lexer[i].compare("\n") != 0){
        if (regex_match(lexer[i], smatch1, variableRegex)){
            Variables::getInstance()->getInterpreter()
                    ->setVariables(lexer[i]+"="+
                    to_string((Variables::getInstance()->getProgMap().find(lexer[i])->second)->getValue()));
        }
        i++;
    }
}
Variables* Variables::instance = 0;





