//
// Created by giladasher on 25/12/2019.
//

#include "OpenDataServerCommand.h"
#include "Data.h"
#include "Lexer.h"
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>


OpenDataServerCommand::OpenDataServerCommand(const string &port) : _port(port) {}

int OpenDataServerCommand::execute(int index, vector<string> &lexer) {
    // checks if expression and calculates it
    Expression *expression = nullptr;
    try {
        expression = Data::getInstance()->getInterpreter()->interpret(lexer[index + 1]);
    } catch (exception &e) {
        cerr << "port is invalid" << endl;
        exit(1);
    }
    double port = expression->calculate();
    delete expression;

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        cerr << "Could not create a socket" << endl;
        exit(1);
    }

    //bind socket to IP address -  we first need to create the sockaddr obj.
    sockaddr_in address{}; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(port); //we need to convert our number to a number that the network understands.

    //the actual bind command
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        cerr << "Could not bind the socket to an IP" << endl;
        exit(1);
    }

    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        cerr << "Error during listening command" << endl;
        exit(1);
    } else {
        cout << "Server is now listening ..." << endl;
    }

    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *) &address,
                               (socklen_t *) &address);

    if (client_socket == -1) {
        cerr << "Error accepting client" << endl;
        exit(1);
    }

    close(socketfd); //closing the listening socket

    //get values from simulator on a separated thread
    thread serverTh(serverThread, client_socket);
    serverTh.detach();
    return 3;
}

void serverThread(int client_socket) {
    //reading from client
    auto data = Data::getInstance();
    vector<string> xmlVariables = data->getXmlVariables();
    string between_lines;

    while (!data->isStop()) {
        // initialize buffer to 0
        char buffer[1187] = {0};

        // read
        int readData = read(client_socket, buffer, 1187);
        int i = 0;
        while( i < readData){
            if (buffer[i] == '\n'){
                vector<string> splittedByCom = Lexer::splitByDelimiter(between_lines, ",");
                // updating the maps
                int k;
                for (k = 0; k < (int) splittedByCom.size(); k++) {
                    //update the key in simMap
                    string s = splittedByCom[k];
                    if (!s.empty()) {
                        data->setValueSimMap(xmlVariables[k], stod(s));
                        //if there is a bind between the maps it updates the value of the varData in progMap
                        if ((data->getBindFromSimMap(xmlVariables[k]) == 1)) {
                            data->setValueProgMap(data->getProgFromSimMap(xmlVariables[k]), (stod(s)));
                        }//
                    }
                }
                between_lines = "";
                i++;
                continue;
            }
            between_lines+= buffer[i];
            i++;
        }
    }
    close(client_socket);
}
