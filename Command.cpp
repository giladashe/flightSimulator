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


using namespace std;

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
    thread serverThread(&OpenDataServerCommand::openServer, this, port);
    // serverThread.join();
    return 3;
}

void OpenDataServerCommand::openServer(double port) {
//create socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        return;
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
        return;
    }

    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr << "Error during listening command" << std::endl;
        //TODO RETURN?
        return;
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }

    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *) &address,
                               (socklen_t *) &address);

    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        //TODO RETURN?
        return;
    }

    close(socketfd); //closing the listening socket
    char buffer[1024];
    //reading from client
    string variables[] = {"airspeed-indicator_indicated-speed-kt", "heading-indicator_offset-deg",
                          "altimeter_indicated-altitude-ft", "altimeter_pressure-alt-ft",
                          "attitude-indicator_indicated-pitch-deg", "attitude-indicator_indicated-roll-deg",
                          "attitude-indicator_internal-pitch-deg", "attitude-indicator_internal-roll-deg",
                          "encoder_indicated-altitude-ft", "encoder_pressure-alt-ft", "gps_indicated-altitude-ft",
                          "gps_indicated-ground-speed-kt", "gps_indicated-vertical-speed", "indicated-heading-deg",
                          "magnetic-compass_indicated-heading-deg", "slip-skid-ball_indicated-slip-skid",
                          "turn-indicator_indicated-turn-rate", "vertical-speed-indicator_indicated-speed-fpm",
                          "flight_aileron", "flight_elevator", "flight_rudder", "flight_flaps", "engine_throttle",
                          "engine_rpm"};
    while (read(client_socket, buffer, 1024) != -1) {
        vector<string> values;
        string valuesStr(buffer);
        values = Lexer::splitByDelimiter(valuesStr, ",");
        //TODO condition of loop
        // int valread = read(client_socket, buffer, 24);
        for (int i = 0; i < 24; i++) {
            //checks if map is empty or if the key isn't in map
            if (Variables::getInstance()->getSimMap().empty() ||
                Variables::getInstance()->getSimMap().find(variables[i]) ==
                Variables::getInstance()->getSimMap().end()) {
                auto data = new VarData(stod(values[i]), "", variables[i], -1);
                Variables::getInstance()->getSimMap().insert(make_pair(variables[i], data));
            } else {
                Variables::getInstance()->getSimMap()[variables[i]]->setValue(stod(values[i]));
            }
            //if there is a bind between the maps it updates the value of the var in progMap
            if (Variables::getInstance()->getSimMap()[variables[i]]->getBind() == 1) {
                Variables::getInstance()->getProgMap()[Variables::getInstance()->getSimMap()[variables[i]]->getProgStr()]->setValue(
                        stod(values[i]));
            }
        }
    }
    std::cout << buffer << std::endl;

    //writing back to client
    char *hello = "Hello, I can hear you! \n";
    send(client_socket, hello, strlen(hello), 0);
    std::cout << "Hello message sent\n" << std::endl;
}

void OpenDataServerCommand::setPort(string port) {
    this->_port = port;
}

ConnectClientCommand::ConnectClientCommand(const string &ip, const string &port) : _ip(ip), _port(port) {}

int ConnectClientCommand::execute(int index, vector<string> &lexer) {
    setPort(lexer[index + 1]);
    double port = Variables::getInstance()->getInterpreter()->interpret(this->_port)->calculate();
    thread clientThread(&ConnectClientCommand::openClient, this, port);
    // check if port is expression
    return 4;
}

void ConnectClientCommand::setPort(string port) {
    this->_port = port;
}

void ConnectClientCommand::openClient(double port) {
//create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        //TODO RETURN?
        return;
    }

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr("127.0.0.1");  //the localhost address
    address.sin_port = htons(port);
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.

    // Requesting a connection with the server on local host with port 8081
    int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server" << std::endl;
        //TODO RETURN?
        return;
    } else {
        std::cout << "Client is now connected to server" << std::endl;
    }

    //if here we made a connection
    char hello[] = "Hi from client";
    int is_sent = send(client_socket, hello, strlen(hello), 0);
    if (is_sent == -1) {
        std::cout << "Error sending message" << std::endl;
    } else {
        std::cout << "Hello message sent to server" << std::endl;
    }

    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);
    std::cout << buffer << std::endl;

    close(client_socket);
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
        int bind;
        if (lexer[index + 2].compare("->") == 0) {
            bind = 1;
        } else {
            bind = 0;
        }
        // create VarData obj and insert to progMap
        VarData *varData = new VarData(-1, progStr, simStr, bind);
        Variables::getInstance()->getProgMap()[progStr] = varData;

        return 6;
    }

        // assignment
    else if (lexer[index + 2].compare("=") == 0) {
        string progStr = lexer[index + 1];
        string simStr = "";
        int bind = -1;
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

    // calculate steps to the next command in "lexer"
    int toJump = 0;
    int j = index;
    while (lexer[j].compare("\n") != 0) {
        j++;
        toJump++;
    }
    return (toJump + 1);
}