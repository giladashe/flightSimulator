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
    //setPort(lexer[index + 1]);
    // check if expression
    double port = Data::getInstance()->getInterpreter()->interpret(lexer[index + 1])->calculate();
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        exit(1);
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

void OpenDataServerCommand::setPort(string port) {
    this->_port = port;
}

void serverThread(int client_socket) {
    //reading from client
    auto data = Data::getInstance();
    string remain; //remain from last read
    char buffer[1187];
    string bufferToString;
    vector<string> valuesLines;

    while (!data->isStop()) {
        // buffer = 0
        int i;
        for (i = 0; i < 1187; i++) {
            buffer[i] = 0;
        }
        int buffer_size = sizeof(buffer) / sizeof(char);

        cout << "before read"<<endl;
        // read
        read(client_socket, buffer, 1187);
        cout << "after read"<<endl;

        if (bufferToString.length() != 0) {
            bufferToString.clear();
        }

        bufferToString = Lexer::convertToString(buffer, buffer_size);

        if (remain.length() > 0) {
            remain += bufferToString;
            bufferToString = remain;
            remain = "";
        }

        // if there is a remain in valueLines - clear
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
            //cout << justInCase<<endl;
            int k;

            // variables is xmlVariables
            vector<string> variables = data->getXmlVariables();



            for (k = 0; k < values.size(); k++) {
                // if simMap is empty or if the key isn't in map, insert new VarData to simMap (from buffer)
                /*if (simMap.empty() || simMap.find(variables[k]) == simMap.end()) {
                    auto var_data = new VarData(stod(values[k]), "", variables[k], 0);
                    data->setSimMap(variables[k], var_data);
                }*/

                // if the key in simMap, update


                data->setValueSimMap(variables[k], stod(values[k]));

                //if there is a bind between the maps it updates the value of the varData in progMap
                if ((data->getBindFromSimMap(variables[k]) == 1)) {
                    data->setValueProgMap(data->getProgFromSimMap(variables[k]), (stod(values[k])));
                }
            }
      }
    }
    close(client_socket);
}