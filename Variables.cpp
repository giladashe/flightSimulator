//
// Created by shaiyis on 12/16/19.
//

#include "Variables.h"
#include <regex>
#include <mutex>

using namespace std;

unordered_map<string, VarData *> &Variables::getProgMap() {
    return this->_progMap;
}

unordered_map<string, VarData *> &Variables::getSimMap() {
    return this->_simMap;
}

void Variables::setSimMap(string key, VarData *varData) {
    this->_simMap.insert(make_pair(key, varData));
}

Variables::Variables() {
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

double Variables::calculate(string s) {
    return this->interpreter->interpret(s)->calculate();
}

Interpreter *Variables::getInterpreter() {
    return this->interpreter;
}


void Variables::updateVariables(int index, vector<string> &lexer) {
    regex variableRegex("[a-z|A-Z|_]+[a-z|A-Z|_|0-9]*");
    smatch smatch1;
    int i = index;

    // update the variables values on "setVariables" at Interpreter
    while (lexer[i + 1] != "\n") {
        if (regex_match(lexer[i + 1], smatch1, variableRegex)) {
            double value = Variables::getInstance()->getProgMap()[lexer[i + 1]]->getValue();
            string variableSet = lexer[i + 1] + "=" + to_string(value);
            Variables::getInstance()->getInterpreter()->setVariables(variableSet);
        }
        i++;
    }
}

Variables *Variables::instance = 0;

thread *Variables::getServerThread() const {
    return _serverThread;
}

thread *Variables::getClientThread() const {
    return _clientThread;
}

void Variables::setServerThread(thread *serverThread) {
    Variables::_serverThread = serverThread;
}

void Variables::setClientThread(thread *clientThread) {
    Variables::_clientThread = clientThread;
}

bool Variables::isStop() const {
    return _stop;
}

void Variables::setStop(bool stop) {
    Variables::_stop = stop;
}

void Variables::updateSimMap(string key, double value) {
    this->_simMap[key]->setValue(value);
}

void Variables::updateProgMap(string key, double value) {
    this->_progMap[key]->setValue(value);
}

void Variables::setCommandMap(const string &key, Command *command) {
    this->_commandMap.insert(make_pair(key, command));
}

void Variables::setProgMap(string key, VarData *varData) {
    this->_progMap.insert(make_pair(key, varData));
}

void Variables::removeFromProgMap(string key) {
    this->_progMap.erase(key);
}

void Variables::updateBindSimMap(string key, int bind) {
    this->_simMap[key]->setBind(bind);
}

const vector<string> &Variables::getXmlVariables() const {
    return xmlVariables;
}

unordered_map<string, Command *> &Variables::getCommandMap() {
    return this->_commandMap;
}
