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
    Expression *expression = nullptr;
    try {
        expression = Data::getInstance()->getInterpreter()->interpret(lexer[index + 2]);
    } catch (exception &e) {
        cerr << "port is invalid" << endl;
        exit(1);
    }
    string ip = lexer[index + 1];
    ip.erase(remove(ip.begin(), ip.end(), '"'), ip.end());
    //calculate the expression in the port
    double port = expression->calculate();
    delete expression;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {//
        //error
        cerr << "Could not create a socket" << endl;
        exit(1);
    }

    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address{}; //in means IP4
    address.sin_family = AF_INET;//IP4
    //we need to convert our number (both port & localhost)
    // to a number that the network understands.
    address.sin_addr.s_addr = inet_addr(ip.c_str());  //the localhost address
    address.sin_port = htons(port);
    // Requesting a connection with the server on local host with port 8081
    int is_connect = 0;
    is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        cerr << "Could not connect" << endl;
        exit(1);
    }
    cout << "Client is now connected to server" << endl;

    //make a thread that runs the loop for sending messages to the simulator
    thread clientTh(clientThread, client_socket);
    //let the thread be independently running
    clientTh.detach();
    //return how much further to go in the lexer tokens array
    return 4;
}

void clientThread(int client_socket) {
    //create socket

    auto data = Data::getInstance();

    while (!data->isStop()) {
        //if here we made a connection
          Data::getInstance()->queueMutex.lock();
        while (!data->commandsQueue.empty()) {
            int is_sent = send(client_socket, data->commandsQueue.front().c_str(),
                               data->commandsQueue.front().length(), 0);
            if (is_sent == -1) {
                cout << "Error sending message" << endl;
            } else {
                data->commandsQueue.pop();
            }
        }
          Data::getInstance()->queueMutex.unlock();
    }
    close(client_socket);
}