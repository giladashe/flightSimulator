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

// singleton Data so all the program has access and it's constructed once


using namespace std;

class Data {
    Data(); // private constructor
    unordered_map<string, VarData *> _progMap; //map for the program variables
    unordered_map<string, VarData *> _simMap; //map for the simulator variables
    unordered_map<string, Command *> _commandMap; //map for the commands that the program can perform
    static Data *instance;
    Interpreter *interpreter;
    bool _stop = false; //boolean to stop the threads
    vector<string> _xmlVariables; //list of the variables from the xml file

public:

    virtual ~Data();

    const vector<string> &getXmlVariables() const;
    //mutex for every map and queue
    mutex simMapMutex;
    mutex progMapMutex;
    mutex queueMutex;
    mutex comMapMutex;

    queue<string> commandsQueue; //queue for the set commands for the simulator
    //get an instance for data - singleton
    static Data *getInstance() {
        if (!instance)
            instance = new Data();
        return instance;
    }

    void * getCommandMap(const string& key);

    Interpreter *getInterpreter();

    bool isStop() const;

    // searches for variables in the current line of lexer and update in "setVariables" of Interpreter
    void updateVariables(int index, vector<string> &lexer);

    double calculate(const string& s); //calculate string representing expression with the interpreter

    void setStop(bool stop);

    void setValueSimMap(const string& key, double value); //update value of specified key in simulator map

    void setValueProgMap(const string& key, double value); //update value of specified key in program map

    void setCommandMap(const string &key, Command *varData); //insert new command

    void setProgMap(const string& key, VarData *varData); //insert new variable to program map

    void removeFromProgMap(const string& key); //remove variable from program map

    void updateBindSimMap(const string& key, int bind); //update bind of specified key in program map

    void setProgStrSimMap(const string& key, const string& progStr);//update program string in simulator map

    double getValFromProgMap(const string& key); //get value from the program map

    string getSimFromProgMap(const string& key); //get simulator path from program map

    int getBindFromProgMap(const string& key); //get bind - 1 for bind to simulator and 0 otherwise

    int getBindFromSimMap(const string& key); //get bind - 1 for bind to program map and 0 otherwise

    string getProgFromSimMap(const string& key); //get program string from simulator map

    static void updateVariablesFromStr(string string1); //updates variables in interpreter from a complex string

    static void setVarsInInterpreter(const string& toUpdate); //set variables in interpreter
};


#endif //FLIGHTSIMULATOR_DATA_H
