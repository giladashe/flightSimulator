//
// Created by shaiyis on 12/13/19.
//

#include <iostream>
#include "Command.h"
#include "VarData.h"
#include "Variables.h"
#include "Lexer.h"
#include <chrono>
#include <thread>
#include <regex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "threads.h"


using namespace std;

//thread thread1;
//thread thread2;

//mutex mutex1;

int Command::execute(int index, vector<string> &lexer) {}


PrintCommand::PrintCommand(const string &str) : str(str) {}

int PrintCommand::execute(int index, vector<string> &lexer) {
    string message = string(lexer[index + 1]);
    vector<string> theMessageWithout = Lexer::splitByDelimiter(message, "\"");
    if (theMessageWithout.size() > 1) {
        message = theMessageWithout[1];
       // message.erase(remove(message.begin(), message.end(), '"'), message.end());
    } else {
        double value = Variables::getInstance()->getProgMap()[message]->getValue();
        message = to_string(value);
    }
    cout << message << endl;
    return 3;
}


SleepCommand::SleepCommand(const string &str) : str(str) {}

int SleepCommand::execute(int index, vector<string> &lexer) {
    std::this_thread::sleep_for(std::chrono::milliseconds(stol(lexer[index + 1])));
    return 3;
}

OpenDataServerCommand::OpenDataServerCommand(const string &port) : _port(port) {}

int OpenDataServerCommand::execute(int index, vector<string> &lexer) {
    setPort(lexer[index + 1]);
    // check if expression
    double port = Variables::getInstance()->getInterpreter()->interpret(this->_port)->calculate();
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
    }

    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address{}; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(port);
    //we need to convert our number
    // to a number that the network understands.

    //the actual bind command
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Could not bind the socket to an IP" << std::endl;
        //TODO RETURN?

    }

    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr << "Error during listening command" << std::endl;
        //TODO RETURN?

    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }

    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *) &address,
                               (socklen_t *) &address);


    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        //TODO RETURN?
    }

    close(socketfd); //closing the listening socket
    serverThread = thread(&OpenDataServerCommand::getFromClient, this, client_socket);
    serverThread.detach();
    //Variables::getInstance()->setServerThread(new thread(&OpenDataServerCommand::getFromClient, this, client_socket));
    // Variables::getInstance()->getServerThread()->detach();
    return 3;
}

void OpenDataServerCommand::getFromClient(int clientSocket) {
//create socket
    //reading from client
    string variables[] = {"/instrumentation/airspeed-indicator/indicated-speed-kt", "/sim/time/warp",
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
    string remain;
    char buffer[1187];
    string bufferToString;
    vector<string> valuesLines;
    while (!Variables::getInstance()->isStop()) {
        this_thread::sleep_for(100ms);
        int i;
        for (i = 0; i < 1187; i++) {
            buffer[i] = 0;
        }
        int buffer_size = sizeof(buffer) / sizeof(char);
        read(clientSocket, buffer, 1187);
        //TODO FUNCTION
        if (bufferToString.length() != 0) {
            bufferToString.clear();
        }
        bufferToString = Lexer::convertToString(buffer, buffer_size);
        if (remain.length() > 0) {
            remain += bufferToString;
            bufferToString = remain;
            remain = "";
        }
        //int size = bufferToString.length();
        // valuesStrLines = valuesStrLines.substr(0,valuesStrLines[valuesStrLines.find('\n')-1]);
        // valuesStrLines.erase(remove(valuesStrLines.begin(), valuesStrLines.end(), '\n'), valuesStrLines.end());
        if (valuesLines.size() != 0) {
            valuesLines.clear();
        }
        valuesLines = Lexer::splitByDelimiter(bufferToString, "\n");
        if (valuesLines[valuesLines.size() - 1] == "") {
            valuesLines.pop_back();
        }

        // for on the lines
        int j = 0;
        for (; j < valuesLines.size(); j++) {
            string justInCase = valuesLines[j];
            vector<string> values = Lexer::splitByDelimiter(valuesLines[j], ",");
            // if less than 36 values erase
            //todo keep?
            /*
            if (values.size() < 36) {
                valuesLines.erase(valuesLines.begin() + j);
            }
             */
            if (values.size() != 36) {
                remain += justInCase;
                break;
            }
            int k;
            if (Variables::m.try_lock()) {
                //TODO UPDATE THE VALUES IN SIMMAP
                for (k = 0; k < values.size(); k++) {
                    //checks if map is empty or if the key isn't in map
                    if (Variables::getInstance()->getSimMap().empty() ||
                        Variables::getInstance()->getSimMap().find(variables[k]) ==
                        Variables::getInstance()->getSimMap().end()) {
                        auto data = new VarData(stod(values[k]), "", variables[k], 0);
                        Variables::getInstance()->setSimMap(variables[k], data);
                    } else {
                        Variables::getInstance()->updateSimMap(variables[k], stod(values[k]));
                    }
                    //if there is a bind between the maps it updates the value of the var in progMap
                    if (Variables::getInstance()->getSimMap()[variables[k]]->getBind() == 1) {
                        Variables::getInstance()->updateProgMap(
                                Variables::getInstance()->getSimMap()[variables[k]]->getProgStr(), (stod(values[k])));
                    }
                }
                Variables::m.unlock();
            }
        }
    }
}

void OpenDataServerCommand::setPort(string port) {
    this->_port = port;
}

ConnectClientCommand::ConnectClientCommand(const string &ip, const string &port) : _ip(ip), _port(port) {}

int ConnectClientCommand::execute(int index, vector<string> &lexer) {
    setIp(lexer[index + 1]);
    this->_ip.erase(remove(_ip.begin(), _ip.end(), '"'), _ip.end());
    setPort(lexer[index + 2]);
    double port = Variables::getInstance()->getInterpreter()->interpret(this->_port)->calculate();
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        //TODO RETURN?
//        return;
    }

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address{}; //in means IP4
    address.sin_family = AF_INET;//IP4
    //todo ip not converted properly
    address.sin_addr.s_addr = inet_addr(this->_ip.c_str());  //the localhost address
    address.sin_port = htons(port);
    // Requesting a connection with the server on local host with port 8081
    int is_connect = 0;
    do {
        is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    } while (is_connect == -1);
    std::cout << "Client is now connected to server" << std::endl;
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.
    clientThread = thread(&ConnectClientCommand::sendMessages, this, client_socket);
    clientThread.detach();
    //Variables::getInstance()->setClientThread(
    //      new thread(&ConnectClientCommand::sendMessages, this, client_socket));
    //Variables::getInstance()->getClientThread()->detach();
    return 4;
}

void ConnectClientCommand::setPort(string port) {
    this->_port = port;
}

void ConnectClientCommand::sendMessages(int clientSocket) {
//create socket


/*
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server" << std::endl;
        //TODO RETURN?
        return;
    } else {
        std::cout << "Client is now connected to server" << std::endl;
    }
    */
    while (!Variables::getInstance()->isStop()) {
        this_thread::sleep_for(100ms);//todo ?
        //if here we made a connection
        if (Variables::m.try_lock()) {
            for (const auto &var: Variables::getInstance()->getSimMap()) {

                string message = "set " + var.second->getSimStr().substr(1, var.second->getSimStr().size() - 1) + " " +
                                 to_string(var.second->getValue()) + "/r/n";
                message.erase(remove(message.begin(), message.end(), '"'), message.end());
                int is_sent = send(clientSocket, message.c_str(), message.length(), 0);
                if (is_sent == -1) {
                    std::cout << "Error sending message" << std::endl;
                } else {
                    std::cout << "message sent to server" << std::endl;
                }
                /*
                char buffer[1024] = {0};
                int valread = read(client_socket, buffer, 1024);
                std::cout << buffer << std::endl;
                 */
            }
            Variables::m.unlock();
        }
    }
    close(clientSocket);
}

void ConnectClientCommand::setIp(const string &ip) {
    _ip = ip;
}

IfCommand::IfCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

int IfCommand::execute(int index, vector<string> &lexer) {
    int stepsToFirstCommand;
    stepsToFirstCommand = this->stepsToFirstCommand(index, lexer);
    int i = index + stepsToFirstCommand;
    int stepOutOfTheScope;
    stepOutOfTheScope = this->stepOutOfTheScope(index, lexer);

    if (this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            Command *command = (Variables::getInstance()->getCommandMap().find(lexer[i]))->second;
            if (command != nullptr) {
                i += command->execute(i, lexer);
            }
            // assignmentCommand
            else{
                Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
                i += assignmentCommand->execute(i, lexer);
            }
            //TODO FREE ALL COMMANDS
        }
        return (stepOutOfTheScope);
    }
}

LoopCommand::LoopCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

using namespace std;
int LoopCommand::execute(int index, vector<string> &lexer) {
    int stepsToFirstCommand;
    stepsToFirstCommand = this->stepsToFirstCommand(index, lexer);
    int i = index + stepsToFirstCommand;
    int stepOutOfTheScope;
    stepOutOfTheScope = this->stepOutOfTheScope(index, lexer);

    while (this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            Command *command = (Variables::getInstance()->getCommandMap().find(lexer[i]))->second;
            if (command != nullptr) {
                i += command->execute(i, lexer);
            }
            // assignmentCommand
            else{
                Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
                i += assignmentCommand->execute(i, lexer);
            }
            //TODO FREE ALL COMMANDS
            i = index + stepsToFirstCommand;
        }
        return (stepOutOfTheScope);
    }
}

ConditionParserCommand::ConditionParserCommand(const string &leftStr, const string &rightStr) : leftStr(leftStr),
                                                                                                rightStr(rightStr) {}

int ConditionParserCommand::execute(int index, vector<string> &lexer) {}

const list<Command> &ConditionParserCommand::getCommandList() const {
    return this->commandList;
}

double ConditionParserCommand::checkCondition(int index, vector<string> &lexer) {
    //if (Variables::m.try_lock()){
    Variables::getInstance()->updateVariables(index, lexer);
    //Variables::m.unlock();
    //}

    // find the operator
    int i = index;
    while ((lexer[i].compare("!=") != 0) && (lexer[i].compare("==") != 0)
           && (lexer[i].compare(">=") != 0) && (lexer[i].compare("<=") != 0)
           && (lexer[i].compare(">") != 0) && (lexer[i].compare("<") != 0)) {
        i++;
    }
    string op = lexer[i];

    // finds leftStr & rightStr of the condition
    int j = index;
    string sidesStr = "";
    while (lexer[j+1] != "{"){
        sidesStr += lexer[j+1];
        j++;
    }
    vector<string> sidesVector = Lexer::splitByDelimiter(sidesStr, op);
    this->leftStr = sidesVector[0];
    this->rightStr = sidesVector[1];

    double leftStrVal = Variables::getInstance()->calculate(this->leftStr);
    double rightStrVal = Variables::getInstance()->calculate(this->rightStr);
    Expression *left = new Value(leftStrVal);
    Expression *right = new Value(rightStrVal);
    Expression *e = new Condition(left, right, op);

    return (e->calculate());
}

int ConditionParserCommand::stepsToFirstCommand(int index, vector<string> &lexer) {
    int i = index;
    int steps = 0;
    while (lexer[i] != "\n"){
        i++;
        steps++;
    }
    return steps+1;
}

int ConditionParserCommand::stepOutOfTheScope(int index, vector<string> &lexer) {
    int i = index;
    int steps = 0;
    while (lexer[i] != "}"){
        i++;
        steps++;
    }
    return steps+2;
}


DefineVarCommand::DefineVarCommand(const string &var, const string &value) : var(var), value(value) {}

int DefineVarCommand::execute(int index, vector<string> &lexer) {
    //if (Variables::m.try_lock()){

    // if it's new Var definition
    if ((lexer[index + 2].compare("->") == 0) || (lexer[index + 2].compare("<-") == 0)) {
        string progStr = lexer[index + 1];
        string simStr = lexer[index + 4];
        simStr.erase(remove(simStr.begin(), simStr.end(), '"'), simStr.end());

        int bindOfVar;
        if (lexer[index + 2].compare("->") == 0) {
            bindOfVar = 1;
        } else {
            bindOfVar = 0;
        }
        // create VarData obj and insert to progMap
        VarData *varData = new VarData(-1, progStr, simStr, bindOfVar);
        Variables::getInstance()->setProgMap(progStr, varData);
        if (Variables::getInstance()->getSimMap()[simStr] == nullptr) {
            int x = 0;
        }
        if (bindOfVar == 0) {
            Variables::getInstance()->getSimMap()[simStr]->setBind(1);
        }
        //Variables::m.unlock();
        return 6;
    }


        // assignment
    else if (lexer[index + 2].compare("=") == 0) {
        //if (Variables::m.try_lock()) {
        string progStr = lexer[index + 1];
        string simStr = "";
        int bind = 0;
        // create VarData obj and insert to progMap
        VarData *varData = new VarData(-1, progStr, simStr, bind);
        Variables::getInstance()->getProgMap()[progStr] = varData;
        Command *assignmentCommand =
                Variables::getInstance()->getCommandMap().find("assign")->second;
        int toJump = assignmentCommand->execute(index + 1, lexer);
        //Variables::m.unlock();
        return toJump + 1;
        //}
    }
}


AssignmentCommand::AssignmentCommand(const string &var) : _var(var) {
}

int AssignmentCommand::execute(int index, vector<string> &lexer) {
    //
    string strToAssign = lexer[index];
    //update setVariables on Interpreter
    //if (Variables::m.try_lock()){
    Variables::getInstance()->updateVariables(index, lexer);

    // string to calculate
    int i = index;
    string strToCalculate;
    while (lexer[i] != "\n") {
        strToCalculate += lexer[i];
        i++;
    }
    vector<string> varAndVal = Lexer::splitByDelimiter(strToCalculate, "=");
    double value = Variables::getInstance()->calculate(varAndVal[1]);

    //todo erase
    //double value = Variables::getInstance()->calculate(strToCalculate);

    // assign the value as was calculated
    Variables::getInstance()->updateProgMap(varAndVal[0], value);

    //todo remove
    //   (Variables::getInstance()->getProgMap().find(strToAssign)->second)->setValue(value);
    if (Variables::getInstance()->getProgMap()[strToAssign]->getBind() == 1) {
        Variables::getInstance()->updateSimMap(Variables::getInstance()->getProgMap()[strToAssign]->getSimStr(), value);
        //todo remove
        //Variables::getInstance()->getSimMap()[strToAssign]->setValue(value);
    }
    // calculate steps to the next command in "lexer"
    int toJump = 0;
    int j = index;
    while (lexer[j] != "\n") {
        j++;
        toJump++;
    }
    //Variables::m.unlock();
    return (toJump + 1);
    //}
}

FuncCommand::FuncCommand(const string &variable) : var(variable) {}

int FuncCommand::execute(int index, vector<string> &lexer) {
    //insert local variable to the map and at the end erase it
    //if (Variables::m.try_lock()){
    Variables::getInstance()->setProgMap(this->var, new VarData(this->_val, "", "", 0));
    this->setVal(stod(lexer[index + 2]));
    int i = this->_startIndex;
    int j = this->_endIndex;
    int jump = 0;
    while (i < j) {
        auto it = Variables::getInstance()->getCommandMap().find(lexer[index]);
        if (it != Variables::getInstance()->getCommandMap().end()) {
            Command *command = it->second;
            i += command->execute(index, lexer);
        } else {
            // assignmentCommand
            Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
            i += assignmentCommand->execute(index, lexer);
        }
    }
    for (jump = 0; lexer[index] != "\n"; jump++) {
        index++;
    }
    Variables::getInstance()->removeFromProgMap(this->var);
    //Variables::m.unlock();
    return jump + 1;
    //}
}

int FuncCommand::getStartIndex() const {
    return _startIndex;
}

int FuncCommand::getEndIndex() const {
    return _endIndex;
}

void FuncCommand::setStartIndex(int startIndex) {
    this->_startIndex = startIndex;
}

void FuncCommand::setEndIndex(int endIndex) {
    this->_endIndex = endIndex;
}

const string &FuncCommand::getVar() const {
    return this->var;
}

void FuncCommand::setVal(double val) {
    this->_val = val;
}

int MakeFuncCommand::execute(int index, vector<string> &lexer) {
    int i = 0;
    i = 3;
    int j = index;
    FuncCommand *funcCommand = new FuncCommand(lexer[j + 3]);
    i += 4;
    j += i;
    funcCommand->setStartIndex(j);
    while (lexer[j] != "}") {
        j++;
        i++;
    }
    funcCommand->setEndIndex(j);
    //if (Variables::m.try_lock()){
    Variables::getInstance()->setCommandMap(lexer[index], funcCommand);
    //Variables::m.unlock();
    return i;
    //}
}

