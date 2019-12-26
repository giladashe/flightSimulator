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
#include <regex>

using namespace std;

unordered_map<string, VarData *> &Data::getProgMap() {
    this->progMapMutex.lock();
    auto progMap = this->_progMap;
    this->progMapMutex.unlock();
    return progMap;
}

unordered_map<string, VarData *> &Data::getSimMap() {
    this->simMapMutex.lock();
    auto simMap = this->_simMap;
    this->simMapMutex.unlock();
    return simMap;
}

void Data::setSimMap(string key, VarData *varData) {
    this->_simMap.insert(make_pair(key, varData));
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
            double value = Data::getInstance()->getProgMap()[lexer[i + 1]]->getValue();
            string variableSet = lexer[i + 1] + "=" + to_string(value);
            Data::getInstance()->getInterpreter()->setVariables(variableSet);
        }
        i++;
    }
}

Data *Data::instance = 0;

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
    this->_progMap[key]->setValue(value);
}

void Data::setCommandMap(const string &key, Command *command) {
    this->_commandMap.insert(make_pair(key, command));
}

void Data::setProgMap(string key, VarData *varData) {
    this->_progMap.insert(make_pair(key, varData));
}

void Data::removeFromProgMap(string key) {
    this->_progMap.erase(key);
}

void Data::updateBindSimMap(string key, int bind) {
    this->_simMap[key]->setBind(bind);
}

const vector<string> &Data::getXmlVariables() const {
    return xmlVariables;
}

unordered_map<string, Command *> &Data::getCommandMap(){
    return this->_commandMap;
}

void Data::setProgStrSimMap(string key, string progStr) {
    this->_simMap[key]->setProgStr(progStr);
}

