//
// Created by giladasher on 25/12/2019.
//

#include <iostream>
#include "ConnectClientCommand.h"
#include "Data.h"
#include <netinet/in.h>
#include <algorithm>
#include <arpa/inet.h>
#include <unistd.h>

ConnectClientCommand::ConnectClientCommand(const string &ip, const string &port) : _ip(ip), _port(port) {}

int ConnectClientCommand::execute(int index, vector<string> &lexer) {
    string ip = lexer[index + 1];
    //setIp(lexer[index + 1]);
    ip.erase(remove(ip.begin(), ip.end(), '"'), ip.end());
    string portStr = lexer[index + 2];
    double port = Data::getInstance()->getInterpreter()->interpret(portStr)->calculate();
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

void clientThread(int client_socket) {
    //create socket

    while (!Data::getInstance()->isStop()) {
        //if here we made a connection
        Data::getInstance()->m.lock();
        while (!Data::getInstance()->commandsQueue.empty()) {
            int is_sent = send(client_socket, Data::getInstance()->commandsQueue.front().c_str(),
                               Data::getInstance()->commandsQueue.front().length(), 0);
            if (is_sent == -1) {
                std::cout << "Error sending message" << std::endl;
            } else {
                std::cout << "message sent to server" << std::endl;
                Data::getInstance()->commandsQueue.pop();
            }
        }
        Data::getInstance()->m.unlock();
    }
    close(client_socket);
}