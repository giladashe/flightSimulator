//
// Created by shaiyis on 12/13/19.
//

#ifndef FLIGHTSIMULATOR_COMMAND_H
#define FLIGHTSIMULATOR_COMMAND_H

#include <string>
#include "ex1.cpp"

using namespace std;

class Command {

public:
    //virtual int execute() = 0;
    virtual ~Command() {}
};

class PrintCommand : public Command{
    string str;
public:
    PrintCommand(string str);
    int execute();

};

class SleepCommand : public Command{
    string str;
public:
    SleepCommand(const string &str);

    SleepCommand();

    int execute();
};

class OpenDataServerCommand : public Command{
public:
    OpenDataServerCommand();
    int execute();
};
class ConnectClientCommand : public Command{
public:
    ConnectClientCommand();
    int execute();
};

class DefineVarCommand : public Command{
    string var;
    string value;
public:
    DefineVarCommand(const string &var, const string &value);

    DefineVarCommand();

    int execute();
};


class ConditionParserCommand : public Command{
protected:
    string leftStr;
    string rightStr;
    list<Command> commandList;
public:
    ConditionParserCommand(const string &leftStr, const string &rightStr);

    ConditionParserCommand();

    int execute();
    const list<Command> &getCommandList() const;
};
class IfCommand : public ConditionParserCommand{
public:
    IfCommand(const string &leftStr, const string &rightStr);
    int execute();

    IfCommand();
};
class LoopCommand : public ConditionParserCommand{
public:
    LoopCommand(const string &leftStr, const string &rightStr);
    int execute();

    LoopCommand();
};
class FuncCommand : public Command{
    string leftStr;
    string rightStr;
    list<Command> commandList;
public:
    FuncCommand(const string &leftStr, const string &rightStr);

    FuncCommand();

    int execute();
    const list<Command> &getCommandList() const;
};


#endif //FLIGHTSIMULATOR_COMMAND_H
