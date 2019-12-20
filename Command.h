//
// Created by shaiyis on 12/13/19.
//

#ifndef FLIGHTSIMULATOR_COMMAND_H
#define FLIGHTSIMULATOR_COMMAND_H

#include <string>
#include <thread>
#include "ex1.h"

using namespace std;

class Command {

public:
    //virtual int execute() = 0;
    virtual ~Command() {}

    virtual int execute(int index, vector<string> &lexer);
};

class PrintCommand : public Command {
    string str;
public:
    PrintCommand(const string &str);

    int execute(int index, vector<string> &lexer);

};

class SleepCommand : public Command {
    string str;
public:
    SleepCommand(const string &str);

    int execute(int index, vector<string> &lexer);
};

class OpenDataServerCommand : public Command {
    string _port;
public:
    OpenDataServerCommand(const string &port);

    int execute(int index, vector<string> &lexer);

    void getFromClient(int clientSocket);

    void setPort(string port);
};

class ConnectClientCommand : public Command {
    string _ip;
    string _port;
public:
    ConnectClientCommand(const string &ip, const string &port);

    int execute(int index, vector<string> &lexer);

    void sendMessages(int clientSocket);

    void setPort(string port);

    void setIp(const string &ip);
};

class DefineVarCommand : public Command {
    string var;
    string value;
public:
    DefineVarCommand(const string &var, const string &value);

    int execute(int index, vector<string> &lexer);
};

// different from Condition (extends BinaryOperator)
class ConditionParserCommand : public Command {
protected:
    string leftStr;
    string rightStr;
    list <Command> commandList;
public:
    ConditionParserCommand(const string &leftStr, const string &rightStr);

    int execute(int index, vector<string> &lexer);

    const list <Command> &getCommandList() const;

    double checkCondition(int index, vector<string> &lexer);
};

class IfCommand : public ConditionParserCommand {
public:
    IfCommand(const string &leftStr, const string &rightStr);

    int execute(int index, vector<string> &lexer);
};

class LoopCommand : public ConditionParserCommand {
public:
    LoopCommand(const string &leftStr, const string &rightStr);

    int execute(int index, vector<string> &lexer);
};

class FuncCommand : public Command {
    string var;
    list <Command> commandList;
public:
    FuncCommand(const string &variable);

    int execute(int index, vector<string> &lexer);

    const list <Command> &getCommandList() const;
};

class AssignmentCommand : public Command {
    string _var;
public:
    AssignmentCommand(const string &var);

    int execute(int index, vector<string> &lexer);
};
//

#endif //FLIGHTSIMULATOR_COMMAND_H
