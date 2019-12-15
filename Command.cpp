//
// Created by shaiyis on 12/13/19.
//

#include <iostream>
#include "Command.h"
#include "VarData.h"
#include <chrono>
#include <thread>

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

}

LoopCommand::LoopCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}
int LoopCommand::execute(int index, vector<string>& lexer) {}

ConditionParserCommand::ConditionParserCommand(const string &leftStr, const string &rightStr) : leftStr(leftStr)
,rightStr(rightStr) {}
int ConditionParserCommand::execute(int index, vector<string>& lexer) {}

const list<Command> &ConditionParserCommand::getCommandList() const {
    return this->commandList;
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
        //TODO insert to singleton
    }


    return 6;
    // if assignment check!
}

