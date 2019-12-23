//
// Created by shaiyis on 12/16/19.
//

#ifndef FLIGHTSIMULATOR_VARIABLES_H
#define FLIGHTSIMULATOR_VARIABLES_H


#include <unordered_map>
#include <thread>
#include <mutex>
#include "VarData.h"
#include "Expression.h"
#include "ex1.h"
#include "Command.h"

// singleton Variables  so all the program has access



class Variables {
    Variables(); // private constructor
    unordered_map<string, VarData *> _progMap;
    unordered_map<string, VarData *> _simMap;
    unordered_map<string, Command *> _commandMap;
    static Variables *instance;
    Interpreter *interpreter;
    thread *_serverThread = nullptr;
    thread *_clientThread = nullptr;
    bool _stop = false;
public:
    static mutex m;

    static Variables *getInstance();

    unordered_map<string, VarData *> getProgMap();

    unordered_map<string, VarData *> getSimMap();

    unordered_map<string, Command *> &getCommandMap();

    Interpreter *getInterpreter();

    bool isStop() const;

    // searches for variables in the curent line and update in "setVariables"
    // of Interpreter
    void updateVariables(int index, vector<string> &lexer);

    int calculate(string s);

    thread *getServerThread() const;

    thread *getClientThread() const;

    void setServerThread(thread *serverThread);

    void setClientThread(thread *clientThread);

    void setStop(bool stop);

    void setSimMap(string key,VarData* varData);

    void updateSimMap(string key,double value);

    void updateProgMap(string key,double value);

    void setCommandMap(const string& key,Command* varData);

    void setProgMap(string key,VarData* varData);

    void removeFromProgMap(string key);

};


#endif //FLIGHTSIMULATOR_VARIABLES_H
