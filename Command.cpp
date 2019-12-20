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
int Command::execute(int index, vector<string> &lexer) {}


PrintCommand::PrintCommand(const string &str) : str(str) {}

int PrintCommand::execute(int index, vector<string> &lexer) {
    cout << lexer[index + 1] << endl;
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
    thread1 = thread(&OpenDataServerCommand::getFromClient, this, client_socket);
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
    while (!Variables::getInstance()->isStop()) {
        //  this_thread::sleep_for(10ms);
        char buffer[10000] = {0};
        read(clientSocket, buffer, 10000);
        //TODO FUNCTION
        string valuesStrLines(buffer);
        // valuesStrLines = valuesStrLines.substr(0,valuesStrLines[valuesStrLines.find('\n')-1]);
        // valuesStrLines.erase(remove(valuesStrLines.begin(), valuesStrLines.end(), '\n'), valuesStrLines.end());
        vector<string> valuesLines = Lexer::splitByDelimiter(valuesStrLines, "\n");
        if (!valuesLines.empty()) {
            valuesLines.pop_back();
        }

        // for on the lines
        int j = 0;
        for (; j < valuesLines.size(); j++) {

            vector<string> values = Lexer::splitByDelimiter(valuesLines[j], ",");
            int i;

            //TODO UPDATE THE VALUES IN SIMMAP
            for (i = 0; i < values.size(); i++) {
                //checks if map is empty or if the key isn't in map
                if (Variables::getInstance()->getSimMap().empty() ||
                    Variables::getInstance()->getSimMap().find(variables[i]) ==
                    Variables::getInstance()->getSimMap().end()) {
                    auto data = new VarData(stod(values[i]), "", variables[i], 0);
                    Variables::getInstance()->setSimMap(variables[i], data);
                } else {
                    Variables::getInstance()->updateSimMap(variables[i], stod(values[i]));
                }
                //if there is a bind between the maps it updates the value of the var in progMap
                if (Variables::getInstance()->getSimMap()[variables[i]]->getBind() == 1) {
                    Variables::getInstance()->updateProgMap(
                            Variables::getInstance()->getSimMap()[variables[i]]->getProgStr(), (stod(values[i])));
                }
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
    address.sin_addr.s_addr = inet_addr(this->_ip.data());  //the localhost address
    address.sin_port = htons(port);
    // Requesting a connection with the server on local host with port 8081
    int is_connect = 0;
    do {
        is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    } while (is_connect == -1);
    std::cout << "Client is now connected to server" << std::endl;
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.
    thread2 = thread(&ConnectClientCommand::sendMessages, this, client_socket);
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
        //    this_thread::sleep_for(10ms);
        //if here we made a connection
        for (const auto &var: Variables::getInstance()->getSimMap()) {
            string message = "set " + var.second->getSimStr() + to_string(var.second->getValue()) + "/r/n";
            int is_sent = send(clientSocket, message.data(), message.size(), 0);
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
    }
    close(clientSocket);
}

void ConnectClientCommand::setIp(const string &ip) {
    _ip = ip;
}

IfCommand::IfCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

int IfCommand::execute(int index, vector<string> &lexer) {
    int i = index;
    if (this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            Command *command = (Variables::getInstance()->getCommandMap().find(lexer[i]))->second;
            if (command != nullptr) {
                i += command->execute(i, lexer);
            }
            // assignmentCommand
            Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
            i += assignmentCommand->execute(i, lexer);
        }
        return (i + 1);
    }
}

LoopCommand::LoopCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

int LoopCommand::execute(int index, vector<string> &lexer) {
    int i = index;
    while (this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            Command *command = (Variables::getInstance()->getCommandMap().find(lexer[i]))->second;
            if (command != nullptr) {
                i += command->execute(i, lexer);
            }
            // assignmentCommand
            Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
            i += assignmentCommand->execute(i, lexer);
        }
        return (i + 1);
    }
}

ConditionParserCommand::ConditionParserCommand(const string &leftStr, const string &rightStr) : leftStr(leftStr),
                                                                                                rightStr(rightStr) {}

int ConditionParserCommand::execute(int index, vector<string> &lexer) {}

const list<Command> &ConditionParserCommand::getCommandList() const {
    return this->commandList;
}

double ConditionParserCommand::checkCondition(int index, vector<string> &lexer) {
    Variables::getInstance()->updateVariables(index, lexer);
    // find the operator
    int i = index;
    while ((lexer[i].compare("!=") != 0) && (lexer[i].compare("==") != 0)
           && (lexer[i].compare(">=") != 0) && (lexer[i].compare("<=") != 0)
           && (lexer[i].compare(">") != 0) && (lexer[i].compare("<") != 0)) {
        i++;
    }
    string op = lexer[i];

    double leftStrVal = Variables::getInstance()->calculate(this->leftStr);
    double rightStrVal = Variables::getInstance()->calculate(this->rightStr);
    Expression *left = new Value(leftStrVal);
    Expression *right = new Value(rightStrVal);
    Expression *e = new Condition(left, right, op);

    return (e->calculate());
}


FuncCommand::FuncCommand(const string &variable) : var(variable) {}

int FuncCommand::execute(int index, vector<string> &lexer) {}

const list<Command> &FuncCommand::getCommandList() const {
    return this->commandList;
}

DefineVarCommand::DefineVarCommand(const string &var, const string &value) : var(var), value(value) {}

int DefineVarCommand::execute(int index, vector<string> &lexer) {

    // if it's new Var definition
    if ((lexer[index + 2].compare("->") == 0) || (lexer[index + 2].compare("<-") == 0)) {
        string progStr = lexer[index + 1];
        string simStr = lexer[index + 4];
        int bindOfVar;
        if (lexer[index + 2].compare("->") == 0) {
            bindOfVar = 1;
        } else {
            bindOfVar = 0;
        }
        // create VarData obj and insert to progMap
        VarData *varData = new VarData(-1, progStr, simStr, bindOfVar);
        Variables::getInstance()->getProgMap()[progStr] = varData;
        if (bindOfVar == 0) {
            Variables::getInstance()->getSimMap()[simStr]->setBind(1);
        }
        return 6;
    }

        // assignment
    else if (lexer[index + 2].compare("=") == 0) {
        string progStr = lexer[index + 1];
        string simStr = "";
        int bind = 0;
        // create VarData obj and insert to progMap
        VarData *varData = new VarData(-1, progStr, simStr, bind);
        Variables::getInstance()->getProgMap()[progStr] = varData;
        Command *assignmentCommand =
                Variables::getInstance()->getCommandMap().find("assign")->second;
        int toJump = assignmentCommand->execute(index + 1, lexer);
        return toJump + 1;
    }
}

AssignmentCommand::AssignmentCommand(const string &var) : _var(var) {
}

int AssignmentCommand::execute(int index, vector<string> &lexer) {
    //
    string strToAssign = lexer[index];
    int i = index;
    //update setVariables on Interpreter
    Variables::getInstance()->updateVariables(index, lexer);

    // string to calculate
    i = index;
    string strToCalculate = "";
    while (lexer[i].compare("\n") != 0) {
        strToCalculate += lexer[i];
        i++;
    }
    double value = Variables::getInstance()->calculate(strToCalculate);

    // assign the value as was calculated
    (Variables::getInstance()->getProgMap().find(strToAssign)->second)->setValue(value);
    if (Variables::getInstance()->getProgMap()[strToAssign]->getBind() == 1) {
        Variables::getInstance()->getSimMap()[strToAssign]->setValue(value);
    }
    // calculate steps to the next command in "lexer"
    int toJump = 0;
    int j = index;
    while (lexer[j].compare("\n") != 0) {
        j++;
        toJump++;
    }
    return (toJump + 1);
}