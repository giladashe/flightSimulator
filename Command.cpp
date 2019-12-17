//
// Created by shaiyis on 12/13/19.
//

#include <iostream>
#include "Command.h"
#include "VarData.h"
#include "Variables.h"
#include <chrono>
#include <thread>
#include <regex>

using namespace std;

int Command::execute(int index, vector<string>& lexer) {}


PrintCommand::PrintCommand(const string &str) : str(str) {}

int PrintCommand::execute(int index, vector<string>& lexer) {
    cout << lexer[index+1] << endl;
    return 3;
}


SleepCommand::SleepCommand(const string &str) : str(str) {}
int SleepCommand::execute(int index, vector<string>& lexer) {
    std::this_thread::sleep_for(std::chrono::milliseconds(stol(lexer[index + 1])));
    return 3;
}

OpenDataServerCommand::OpenDataServerCommand(const string &port):_port(port) {}

int OpenDataServerCommand::execute(int index, vector<string>& lexer) {
    // check if expression
    return 3;
}

ConnectClientCommand::ConnectClientCommand(const string &ip,const string &port): _ip(ip), _port(port){}
int ConnectClientCommand::execute(int index, vector<string>& lexer) {
    // check if port is expression
    return 4;
}

IfCommand::IfCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

int IfCommand::execute(int index, vector<string>& lexer) {
    //count steps to return in a loop
    if (this->checkCondition(index, lexer)){
    //for each commands
    }
    // return countsteps;

}

LoopCommand::LoopCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}
int LoopCommand::execute(int index, vector<string>& lexer) {
    //count steps to return in a loop
    while (this->checkCondition(index, lexer)){
        //for each commands
    }
   // return countsteps;
}

ConditionParserCommand::ConditionParserCommand(const string &leftStr, const string &rightStr) : leftStr(leftStr)
,rightStr(rightStr) {}
int ConditionParserCommand::execute(int index, vector<string>& lexer) {}

const list<Command> &ConditionParserCommand::getCommandList() const {
    return this->commandList;
}

double ConditionParserCommand::checkCondition(int index, vector<string> &lexer) {
    Variables::getInstance()->updateVariables(index, lexer);
    // find the operator
    int i = index;
    while((lexer[i].compare("!=") != 0) && (lexer[i].compare("==") != 0)
          && (lexer[i].compare(">=") != 0) && (lexer[i].compare("<=") != 0)
          && (lexer[i].compare(">") != 0) && (lexer[i].compare("<") != 0)){
        i++;
    }
    string op = lexer[i];

    double leftStrVal = Variables::getInstance()->calculate(this->leftStr);
    double rightStrVal = Variables::getInstance()->calculate(this->rightStr);
    Expression* left = new Value(leftStrVal);
    Expression* right = new Value(rightStrVal);
    Expression* e = new Condition(left, right, op);

    return (e->calculate());
}


FuncCommand::FuncCommand(const string &variable) : var(variable) {}
int FuncCommand::execute(int index, vector<string>& lexer) {}

const list<Command> &FuncCommand::getCommandList() const {
    return this->commandList;
}

DefineVarCommand::DefineVarCommand(const string &var, const string &value) : var(var), value(value) {}
int DefineVarCommand::execute(int index, vector<string>& lexer) {

    // if it's new Var definition
    if ((lexer[index+2].compare("->") == 0) || (lexer[index+2].compare("<-") == 0)){
        string progStr = lexer[index+1];
        string simStr = lexer[index+4];
        int bind;
        if (lexer[index+2].compare("->") == 0){
            bind = 1;
        } else{
            bind = 0;
        }
        // create VarData obj and insert to progMap
        VarData* varData = new VarData(-1 , progStr, simStr, bind);
        Variables::getInstance()->getProgMap()[progStr] = varData;

        return 6;
    }

    // assignment
    else if(lexer[index+2].compare("=") == 0) {
        string progStr = lexer[index+1];
        string simStr = "";
        int bind = -1;
        // create VarData obj and insert to progMap
        VarData* varData = new VarData(-1 , progStr, simStr, bind);
        Variables::getInstance()->getProgMap()[progStr] = varData;
        Command* assignmentCommand =
                Variables::getInstance()->getCommandMap().find("assign")->second;
        int toJump = assignmentCommand->execute(index+1, lexer);
        return toJump + 1;
    }
}

AssignmentCommand::AssignmentCommand(const string &var): _var(var) {
}
int AssignmentCommand::execute(int index, vector<string> &lexer) {
    string strToAssign = lexer[index];
    int i = index;
    Variables::getInstance()->updateVariables(index, lexer);

    // string to calculate
    i = index;
    string strToCalculate = "";
    while(lexer[i].compare("\n") != 0){
        strToCalculate += lexer[i];
        i++;
    }
    double value = Variables::getInstance()->calculate(strToCalculate);

    // assign the value as was calculated
    (Variables::getInstance()->getProgMap().find(strToAssign)->second)->setValue(value);

    // calculate steps to the next command in "lexer"
    int toJump = 0;
    int j = index;
    while (lexer[j].compare("\n") != 0) {
        j++;
        toJump++;
    }
    return (toJump+1);
}