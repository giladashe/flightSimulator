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
    string remain; //remain from last read
    char buffer[1187] = {0};
    string bufferToString;
    vector<string> valuesLines;

    while (!data->isStop()) {
        // initialize buffer to 0
        int i;
        for (i = 0; i < 1187; i++) {
            buffer[i] = 0;
        }

        // read
        read(client_socket, buffer, 1187);

        //if the string is not empty it clears it
        if (bufferToString.length() != 0) {
            bufferToString.clear();
        }

        bufferToString = buffer;

        //if there were values from previous read it concatenates them to this read
        if (remain.length() > 0) {
            remain += bufferToString;
            bufferToString = remain;
            remain = "";
        }

        // if there is a remain in valueLines - clear
        if (!valuesLines.empty()) {
            valuesLines.clear();
        }

        //split the read to lines
        valuesLines = Lexer::splitByDelimiter(bufferToString, "\n");
        if (valuesLines[valuesLines.size() - 1].empty()) {
            valuesLines.pop_back();
        }

        // loop on the lines that sets values in the maps
        int j = 0;
        for (; j < (int) valuesLines.size(); j++) {
            //save the line in different string because split changes it
            string justInCase = valuesLines[j];
            vector<string> values = Lexer::splitByDelimiter(valuesLines[j], ",");
            // if it's not enough variables(36) put all the values on the side for next iteration
            if (values.size() != 36) {
                remain += justInCase;
                break;
            }
            int k;

            // variables is _xmlVariables
            vector<string> variables = data->getXmlVariables();


            for (k = 0; k < (int) values.size(); k++) {
                //update the key in simMap

                string s = values[k];
                if (!s.empty()) {
                    data->setValueSimMap(variables[k], stod(s));
                    //if there is a bind between the maps it updates the value of the varData in progMap
                    if ((data->getBindFromSimMap(variables[k]) == 1)) {
                        data->setValueProgMap(data->getProgFromSimMap(variables[k]), (stod(s)));
                    }
                }
            }
        }
    }
    close(client_socket);
}