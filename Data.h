//
// Created by shaiyis on 12/16/19.
//
//
#ifndef FLIGHTSIMULATOR_DATA_H
#define FLIGHTSIMULATOR_DATA_H


#include <unordered_map>
#include <thread>
#include <mutex>
#include "VarData.h"
#include "Expression.h"
#include "ex1.h"
#include "Command.h"

// singleton Data  so all the program has access


using namespace std;

class Data {
    Data(); // private constructor
    unordered_map<string, VarData *> _progMap;
    unordered_map<string, VarData *> _simMap;
    unordered_map<string, Command *> _commandMap;
    static Data *instance;
    Interpreter *interpreter;
    bool _stop = false;
    vector<string> xmlVariables;

public:
    const vector<string> &getXmlVariables() const;
    mutex m;
    queue<string> commandsQueue;
    static Data *getInstance() {
        if (!instance)
            instance = new Data();
        return instance;
    }

    unordered_map<string, VarData *> &getProgMap();

    unordered_map<string, VarData *> &getSimMap();

    unordered_map<string, Command *> &getCommandMap();

    Interpreter *getInterpreter();

    bool isStop() const;

    // searches for variables in the current line and update in "setVariables"
    // of Interpreter
    void updateVariables(int index, vector<string> &lexer);

    double calculate(string s);

    void setStop(bool stop);

    void setSimMap(string key, VarData *varData);

    void setValueSimMap(string key, double value);

    void setValueProgMap(string key, double value);

    void setCommandMap(const string &key, Command *varData);

    void setProgMap(string key, VarData *varData);

    void removeFromProgMap(string key);

    void updateBindSimMap(string key, int bind);

    void setProgStrSimMap(string key, string progStr);

};


#endif //FLIGHTSIMULATOR_DATA_H
