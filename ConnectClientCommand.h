//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_CONNECTCLIENTCOMMAND_H
#define FLIGHTSIMULATOR_CONNECTCLIENTCOMMAND_H


#include "Command.h"

class ConnectClientCommand : public Command {
    string _ip;
    string _port;
public:

    ConnectClientCommand(const string &ip, const string &port);

    int execute(int index, vector<string> &lexer);

    void sendMessages(int clientSocket);

    void setPort(string port);

    void setIp(const string &ip);
};

void clientThread(int client_socket);


#endif //FLIGHTSIMULATOR_CONNECTCLIENTCOMMAND_H
