//
// Created by shaiyis on 12/16/19.
//
//
#include "Data.h"
#include "OpenDataServerCommand.h"
#include "ConnectClientCommand.h"
#include "DefineVarCommand.h"
#include "PrintCommand.h"
#include "SleepCommand.h"
#include "IfCommand.h"
#include "LoopCommand.h"
#include "AssignmentCommand.h"
#include "Lexer.h"
#include <regex>
#include <iostream>

using namespace std;

double Data::getValFromProgMap(string key) {
    this->progMapMutex.lock();
    auto value = this->_progMap[key]->getValue();
    this->progMapMutex.unlock();
    return value;
}

double Data::getValFromSimMap(string key) {
    this->simMapMutex.lock();
    double value = this->_simMap[key]->getValue();
    this->simMapMutex.unlock();
    return value;
}

string Data::getProgFromSimMap(string key) {
    this->simMapMutex.lock();
    string progStr = this->_simMap[key]->getProgStr();
    this->simMapMutex.unlock();
    return progStr;
}


string Data::getSimFromProgMap(string key) {
    this->progMapMutex.lock();
    string sim = this->_progMap[key]->getSimStr();
    this->progMapMutex.unlock();
    return sim;
}

int Data::getBindFromProgMap(string key) {
    this->progMapMutex.lock();
    int bind = this->_progMap[key]->getBind();
    this->progMapMutex.unlock();
    return bind;
}

int Data::getBindFromSimMap(string key) {
    this->simMapMutex.lock();
    int bind = this->_simMap[key]->getBind();
    this->simMapMutex.unlock();
    return bind;
}

void Data::setSimMap(string key, VarData *varData) {
    this->simMapMutex.lock();
    this->_simMap.insert(make_pair(key, varData));
    this->simMapMutex.unlock();
}

Data::Data() {
    this->interpreter = new Interpreter();
    Command *openDataServer = new OpenDataServerCommand("");
    Command *connectClientCommand = new ConnectClientCommand("", "");
    Command *defineVar = new DefineVarCommand("", "");
    Command *printCommand = new PrintCommand("");
    Command *sleepCommand = new SleepCommand("");
    Command *ifCommand = new IfCommand("", "");
    Command *loopCommand = new LoopCommand("", "");
    Command *assignmentCommand = new AssignmentCommand("");
    this->_commandMap["openDataServer"] = openDataServer;
    this->_commandMap["connectControlClient"] = connectClientCommand;
    this->_commandMap["var"] = defineVar;
    this->_commandMap["Print"] = printCommand;
    this->_commandMap["Sleep"] = sleepCommand;
    this->_commandMap["if"] = ifCommand;
    this->_commandMap["while"] = loopCommand;
    this->_commandMap["assign"] = assignmentCommand;
    this->xmlVariables = {"/instrumentation/airspeed-indicator/indicated-speed-kt", "/sim/time/warp",
                          "/controls/switches/magnetos", "/instrumentation/heading-indicator/offset-deg",
                          "/instrumentation/altimeter/indicated-altitude-ft",
                          "/instrumentation/altimeter/pressure-alt-ft",
                          "/instrumentation/attitude-indicator/indicated-pitch-deg",
                          "/instrumentation/attitude-indicator/indicated-roll-deg",
                          "/instrumentation/attitude-indicator/internal-pitch-deg",
                          "/instrumentation/attitude-indicator/internal-roll-deg",
                          "/instrumentation/encoder/indicated-altitude-ft",
                          "/instrumentation/encoder/pressure-alt-ft", "/instrumentation/gps/indicated-altitude-ft",
                          "/instrumentation/gps/indicated-ground-speed-kt",
                          "/instrumentation/gps/indicated-vertical-speed",
                          "/instrumentation/heading-indicator/indicated-heading-deg",
                          "/instrumentation/magnetic-compass/indicated-heading-deg",
                          "/instrumentation/slip-skid-ball/indicated-slip-skid",
                          "/instrumentation/turn-indicator/indicated-turn-rate",
                          "/instrumentation/vertical-speed-indicator/indicated-speed-fpm",
                          "/controls/flight/aileron", "/controls/flight/elevator", "/controls/flight/rudder",
                          "/controls/flight/flaps",
                          "/controls/engines/engine/throttle", "/controls/engines/current-engine/throttle",
                          "/controls/switches/master-avionics", "/controls/switches/starter",
                          "/engines/active-engine/auto-start",
                          "/controls/flight/speedbrake", "/sim/model/c172p/brake-parking",
                          "/controls/engines/engine/primer",
                          "/controls/engines/current-engine/mixture", "/controls/switches/master-bat",
                          "/controls/switches/master-alt",
                          "/engines/engine/rpm"};
    for (const auto &variable : xmlVariables) {
        this->_simMap[variable] = new VarData(0, "", variable, 0);
    }

}

double Data::calculate(string s) {
    return this->interpreter->interpret(s)->calculate();
}

Interpreter *Data::getInterpreter() {
    return this->interpreter;
}


void Data::updateVariables(int index, vector<string> &lexer) {
    regex variableRegex("[a-z|A-Z|_]+[a-z|A-Z|_|0-9]*");
    smatch smatch1;
    int i = index;

    // update the variables values on "setVariables" at Interpreter
    while (lexer[i + 1] != "\n") {
        if (regex_match(lexer[i + 1], smatch1, variableRegex)) {
            double value = Data::getInstance()->getValFromProgMap(lexer[i + 1]);
            string variableSet = lexer[i + 1] + "=" + to_string(value);
            this->getInterpreter()->setVariables(variableSet);
        }
        i++;
    }
}

Data *Data::instance = 0;

void Data::updateVariablesFromStr(string string1) {

    regex variableRegex("[a-z|A-Z|_]+[a-z|A-Z|_|0-9]*");
    smatch smatch1;
    int i;
    string toUpdate;
    for (i = 0; i < (int)string1.length(); i++) {
        if (isalpha(string1[i]) || string1[i] == '_' || (!toUpdate.empty() && isdigit(string1[i]))) {
            toUpdate += string1[i];
            if (i == (int)string1.length() - 1) {
                Data::setVarsInInterpreter(toUpdate);
            }
        } else if (!toUpdate.empty()) {
            Data::setVarsInInterpreter(toUpdate);
            toUpdate.clear();
        }
    }
}

void Data::setVarsInInterpreter(string toUpdate) {
    auto data = Data::getInstance();
    double value;
    regex variableRegex("[a-z|A-Z|_]+[a-z|A-Z|_|0-9]*");
    smatch smatch1;
    if (regex_match(toUpdate, smatch1, variableRegex)) {
        value = data->getValFromProgMap(toUpdate);
        data->getInterpreter()->setVariables(toUpdate + "=" + to_string(value));
    } else {
        cerr << "not valid variable" << endl;
        exit(1);
    }
}


bool Data::isStop() const {
    return _stop;
}

void Data::setStop(bool stop) {
    Data::_stop = stop;
}

void Data::setValueSimMap(string key, double value) {
    this->simMapMutex.lock();
    this->_simMap[key]->setValue(value);
    this->simMapMutex.unlock();
}

void Data::setValueProgMap(string key, double value) {
    this->progMapMutex.lock();
    this->_progMap[key]->setValue(value);
    this->progMapMutex.unlock();
}

void Data::setCommandMap(const string &key, Command *command) {
    this->comMapMutex.lock();
    this->_commandMap.insert(make_pair(key, command));
    this->comMapMutex.unlock();
}

void Data::setProgMap(string key, VarData *varData) {
    this->progMapMutex.lock();
    this->_progMap.insert(make_pair(key, varData));
    this->progMapMutex.unlock();
}

void Data::removeFromProgMap(string key) {
    this->progMapMutex.lock();
    this->_progMap.erase(key);
    this->progMapMutex.unlock();
}

void Data::updateBindSimMap(string key, int bind) {
    this->simMapMutex.lock();
    this->_simMap[key]->setBind(bind);
    this->simMapMutex.unlock();
}

const vector<string> &Data::getXmlVariables() const {
    return xmlVariables;
}

void *Data::getCommandMap(string key) {
    this->comMapMutex.lock();
    if (this->_commandMap.find(key) == this->_commandMap.end()) {
        this->comMapMutex.unlock();
        return nullptr;
    }
    auto command = this->_commandMap[key];
    this->comMapMutex.unlock();

    return command; //todo ?
}

void Data::setProgStrSimMap(string key, string progStr) {
    this->simMapMutex.lock();
    this->_simMap[key]->setProgStr(progStr);
    this->simMapMutex.unlock();
}

Data::~Data() {
    //delete all commands
    for (const auto &command:this->_commandMap) {
        delete command.second;
    }
    delete this->interpreter;
    //delete all var data
    for (const auto &sim:this->_simMap) {
        delete sim.second;
    }
    for (const auto &prog:this->_progMap) {
        delete prog.second;
    }
}

