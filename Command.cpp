//
// Created by shaiyis on 12/13/19.
//

#include "Command.h"

PrintCommand::PrintCommand(string str) : str(str) {}
int PrintCommand::execute() {}


SleepCommand::SleepCommand(const string &str) : str(str) {}
int SleepCommand::execute() {}

SleepCommand::SleepCommand() {}

OpenDataServerCommand::OpenDataServerCommand() {}
int OpenDataServerCommand::execute() {}

ConnectClientCommand::ConnectClientCommand() {}
int ConnectClientCommand::execute() {}

IfCommand::IfCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}
int IfCommand::execute() {}

IfCommand::IfCommand() {}

LoopCommand::LoopCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}
int LoopCommand::execute() {}

LoopCommand::LoopCommand() {}

ConditionParserCommand::ConditionParserCommand(const string &leftStr, const string &rightStr) : leftStr(leftStr)
,rightStr(rightStr) {}
int ConditionParserCommand::execute() {}

const list<Command> &ConditionParserCommand::getCommandList() const {
    return this->commandList;
}

ConditionParserCommand::ConditionParserCommand() {}

FuncCommand::FuncCommand(const string &leftStr, const string &rightStr) : leftStr(leftStr), rightStr(rightStr) {}
int FuncCommand::execute() {}

const list<Command> &FuncCommand::getCommandList() const {
    return this->commandList;
}

FuncCommand::FuncCommand() {}

DefineVarCommand::DefineVarCommand(const string &var, const string &value) : var(var), value(value) {}
int DefineVarCommand::execute() {}

DefineVarCommand::DefineVarCommand() {}

