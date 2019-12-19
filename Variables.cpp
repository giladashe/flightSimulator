//
// Created by shaiyis on 12/16/19.
//

#include "Variables.h"
#include <regex>


unordered_map<string, VarData *> Variables::getProgMap() {
    return this->_progMap;
}

unordered_map<string, VarData *> Variables::getSimMap() {
    return this->_simMap;
}

void Variables::setSimMap(string key, VarData *varData) {
    this->_simMap.insert(make_pair(key, varData));
}

Variables *Variables::getInstance() {
    if (instance == 0) {
        instance = new Variables();
    }
    return instance;
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
}

int Variables::calculate(string s) {
    return this->interpreter->interpret(s)->calculate();
}

Interpreter *Variables::getInterpreter() {
    return this->interpreter;
}

unordered_map<string, Command *> &Variables::getCommandMap() {
    return this->_commandMap;
}

void Variables::updateVariables(int index, vector<string> &lexer) {
    regex variableRegex("[a-z|A-Z|_]+[a-z|A-Z|_|0-9]*");
    smatch smatch1;
    int i = index;

    // update the variables values on "setVariables" at Interpreter
    while (lexer[i].compare("\n") != 0) {
        if (regex_match(lexer[i], smatch1, variableRegex)) {
            Variables::getInstance()->getInterpreter()
                    ->setVariables(lexer[i] + "=" +
                                   to_string((Variables::getInstance()->getProgMap().find(
                                           lexer[i])->second)->getValue()));
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






