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

//mutex m;

void serverThread(int client_socket);
void clientThread(int client_socket);


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
    //setPort(lexer[index + 1]);
    // check if expression
    double port = Variables::getInstance()->getInterpreter()->interpret(lexer[index + 1])->calculate();
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
        exit(1);
    }

    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr << "Error during listening command" << std::endl;
        exit(1);
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }

    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *) &address,
                               (socklen_t *) &address);

    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        exit(1);
    }

    close(socketfd); //closing the listening socket

    //get from client


    thread serverTh(serverThread, client_socket);
    serverTh.detach();
    return 3;
}

void OpenDataServerCommand::getFromClient(int clientSocket) {

}

void OpenDataServerCommand::setPort(string port) {
    this->_port = port;
}

ConnectClientCommand::ConnectClientCommand(const string &ip, const string &port) : _ip(ip), _port(port) {}

int ConnectClientCommand::execute(int index, vector<string> &lexer) {
    string ip = lexer[index + 1];
    //setIp(lexer[index + 1]);
    ip.erase(remove(ip.begin(), ip.end(), '"'), ip.end());
    string portStr = lexer[index + 2];
    double port = Variables::getInstance()->getInterpreter()->interpret(portStr)->calculate();
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        exit(1);
    }

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address{}; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr(ip.c_str());  //the localhost address
    address.sin_port = htons(port);
    // Requesting a connection with the server on local host with port 8081
    int is_connect = 0;
    is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect" << std::endl;
        exit(1);
    }
    std::cout << "Client is now connected to server" << std::endl;
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.

    thread clientTh(clientThread, client_socket);
    clientTh.detach();
    //clientThread = thread(&ConnectClientCommand::sendMessages, this, client_socket);
    //clientThread.detach();
    return 4;
}

void ConnectClientCommand::setPort(string port) {
    this->_port = port;
}

void ConnectClientCommand::sendMessages(int clientSocket) {
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

    if ((bool) this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            Command *command = (Variables::getInstance()->getCommandMap().find(lexer[i]))->second;
            if (command != nullptr) {
                i += command->execute(i, lexer);
            }
                // assignmentCommand
            else {
                Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
                i += assignmentCommand->execute(i, lexer);
            }
            //TODO FREE ALL COMMANDS
        }
    }
    return stepOutOfTheScope;
}

LoopCommand::LoopCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

int LoopCommand::execute(int index, vector<string> &lexer) {
    int stepsToFirstCommand;
    stepsToFirstCommand = this->stepsToFirstCommand(index, lexer);
    int i = index + stepsToFirstCommand;
    int stepOutOfTheScope;
    stepOutOfTheScope = this->stepOutOfTheScope(index, lexer);

    while ((bool) this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            if(Variables::getInstance()->getCommandMap().count(lexer[i])> 0){
                Command *command = (Variables::getInstance()->getCommandMap().find(lexer[i]))->second;
                if (command != nullptr) {
                    i += command->execute(i, lexer);
                }
            }
                // assignmentCommand
            else {
                Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
                i += assignmentCommand->execute(i, lexer);
            }
            //TODO FREE ALL COMMANDS
        }
        i = index + stepsToFirstCommand;
    }
    return stepOutOfTheScope;
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
    while ((lexer[i] != "!=") && (lexer[i] != "==")
           && (lexer[i] != ">=") && (lexer[i] != "<=")
           && (lexer[i] != ">") && (lexer[i] != "<")) {
        i++;
    }
    string op = lexer[i];

    // finds leftStr & rightStr of the condition
    int j = index;
    string sidesStr;
    while (lexer[j + 1] != "{") {
        sidesStr += lexer[j + 1];
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
    while (lexer[i] != "\n") {
        i++;
        steps++;
    }
    return steps + 1;
}

int ConditionParserCommand::stepOutOfTheScope(int index, vector<string> &lexer) {
    int i = index;
    int steps = 0;
    while (lexer[i] != "}") {
        i++;
        steps++;
    }
    return steps + 2;
}


DefineVarCommand::DefineVarCommand(const string &var, const string &value) : var(var), value(value) {}

int DefineVarCommand::execute(int index, vector<string> &lexer) {

    // if it's new Var definition
    if ((lexer[index + 2] == "->") || (lexer[index + 2] == "<-")) {
        string progStr = lexer[index + 1];
        string simStr = lexer[index + 4];
        simStr.erase(remove(simStr.begin(), simStr.end(), '"'), simStr.end());

        int bindOfVar;
        if (lexer[index + 2] == "->") {
            bindOfVar = 1;
        } else {
            bindOfVar = 0;
        }
        // create VarData obj and insert to progMap
        Variables::getInstance()->m.lock();
        auto *varData = new VarData(0, progStr, simStr, bindOfVar);
        Variables::getInstance()->setProgMap(progStr, varData);
        if (bindOfVar == 0) {
            Variables::getInstance()->updateBindSimMap(simStr, 1);
        }
        Variables::getInstance()->m.unlock();
        return 6;
    }

        // assignment
    else {
        string progStr = lexer[index + 1];
        string simStr = "";
        int bind = 0;
        // create VarData obj and insert to progMap
        VarData *varData = new VarData(0, progStr, simStr, bind);
        Variables::getInstance()->m.lock();
        Variables::getInstance()->setProgMap(progStr, varData);

        Command *assignmentCommand = Variables::getInstance()->getCommandMap().find("assign")->second;
        Variables::getInstance()->m.unlock();
        int toJump = assignmentCommand->execute(index + 1, lexer);
        return toJump + 1;
    }
}


AssignmentCommand::AssignmentCommand(const string &var) : _var(var) {
}

int AssignmentCommand::execute(int index, vector<string> &lexer) {
    //
    string strToAssign = lexer[index];
    //update setVariables on Interpreter
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

    // assign the value as was calculated
    Variables::getInstance()->m.lock();
    Variables::getInstance()->updateProgMap(varAndVal[0], value);

    if (Variables::getInstance()->getProgMap()[strToAssign]->getBind() == 1) {
        Variables::getInstance()->updateSimMap(Variables::getInstance()->getProgMap()[strToAssign]->getSimStr(), value);
        string simStr = Variables::getInstance()->getProgMap()[strToAssign]->getSimStr();
        string message = "set " + simStr.substr(1, simStr.size() - 1) + " " + to_string(value) + " /r/n";
        message.erase(remove(message.begin(), message.end(), '"'), message.end());
        Variables::getInstance()->commandsQueue.push(message);

    }
    Variables::getInstance()->m.unlock();

    // calculate steps to the next command in "lexer"
    int toJump = 0;
    int j = index;
    while (lexer[j] != "\n") {
        j++;
        toJump++;
    }
    return (toJump + 1);
}

FuncCommand::FuncCommand(const string &variable) : var(variable) {}

int FuncCommand::execute(int index, vector<string> &lexer) {
    //insert local variable to the map and at the end erase it
    Variables::getInstance()->m.lock();
    Variables::getInstance()->setProgMap(this->var, new VarData(this->_val, "", "", 0));
    this->setVal(stod(lexer[index + 2]));
    Variables::getInstance()->m.unlock();

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
    Variables::getInstance()->m.lock();
    Variables::getInstance()->removeFromProgMap(this->var);
    Variables::getInstance()->m.unlock();

    return jump + 1;
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
    auto *funcCommand = new FuncCommand(lexer[j + 3]);
    i += 4;
    j += i;
    funcCommand->setStartIndex(j);
    while (lexer[j] != "}") {
        j++;
        i++;
    }
    funcCommand->setEndIndex(j);
    Variables::getInstance()->m.lock();
    Variables::getInstance()->setCommandMap(lexer[index], funcCommand);
    Variables::getInstance()->m.unlock();

    return i;
}

void serverThread(int client_socket){
    //reading from client
    string remain;
    char buffer[1187];
    string bufferToString;
    vector<string> valuesLines;
    while (!Variables::getInstance()->isStop()) {
        int i;
        for (i = 0; i < 1187; i++) {
            buffer[i] = 0;
        }
        int buffer_size = sizeof(buffer) / sizeof(char);
        read(client_socket, buffer, 1187);
        if (bufferToString.length() != 0) {
            bufferToString.clear();
        }
        bufferToString = Lexer::convertToString(buffer, buffer_size);
        if (remain.length() > 0) {
            remain += bufferToString;
            bufferToString = remain;
            remain = "";
        }

        if (!valuesLines.empty()) {
            valuesLines.clear();
        }
        valuesLines = Lexer::splitByDelimiter(bufferToString, "\n");
        if (valuesLines[valuesLines.size() - 1].empty()) {
            valuesLines.pop_back();
        }

        // for on the lines
        int j = 0;
        for (; j < valuesLines.size(); j++) {
            string justInCase = valuesLines[j];
            vector<string> values = Lexer::splitByDelimiter(valuesLines[j], ",");
            // put all the values on the side for next iteration
            if (values.size() != 36) {
                remain += justInCase;
                break;
            }
            int k;
            Variables::getInstance()->m.lock();
            for (k = 0; k < values.size(); k++) {
                //checks if map is empty or if the key isn't in map

                // variables is xmlVariables
                vector<string> variables = Variables::getInstance()->getXmlVariables();
                if (Variables::getInstance()->getSimMap().empty() ||
                    Variables::getInstance()->getSimMap().find(variables[k]) ==
                    Variables::getInstance()->getSimMap().end()) {
                    auto data = new VarData(stod(values[k]), "", variables[k], 0);
                    Variables::getInstance()->setSimMap(variables[k], data);
                } else {
                    Variables::getInstance()->updateSimMap(variables[k], stod(values[k]));
                }
                //if there is a bind between the maps it updates the value of the var in progMap
                if ((Variables::getInstance()->getSimMap()[variables[k]]->getBind() == 1) &&
                (Variables::getInstance()->getProgMap().find(
                        Variables::getInstance()->getSimMap()[variables[k]]->getProgStr()) !=
                        Variables::getInstance()->getProgMap().end())){
                        Variables::getInstance()->updateProgMap(Variables::getInstance()->getSimMap()[variables[k]]->getProgStr(), (stod(values[k])));
                }
            }
            Variables::getInstance()->m.unlock();

        }
    }

}
void clientThread(int client_socket){
    //create socket

    while (!Variables::getInstance()->isStop()) {
        //if here we made a connection
        Variables::getInstance()->m.lock();
            while (!Variables::getInstance()->commandsQueue.empty()) {
                int is_sent = send(client_socket, Variables::getInstance()->commandsQueue.front().c_str(),
                                   Variables::getInstance()->commandsQueue.front().length(), 0);
                if (is_sent == -1) {
                    std::cout << "Error sending message" << std::endl;
                } else {
                    std::cout << "message sent to server" << std::endl;
                    Variables::getInstance()->commandsQueue.pop();
                }
            }
        Variables::getInstance()->m.unlock();

    }
    close(client_socket);
}
