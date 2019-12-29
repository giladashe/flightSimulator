//
// Created by giladasher on 25/12/2019.
//

#ifndef FLIGHTSIMULATOR_OPENDATASERVERCOMMAND_H
#define FLIGHTSIMULATOR_OPENDATASERVERCOMMAND_H


#include "Command.h"

class OpenDataServerCommand : public Command {
    string _port;
public:
    OpenDataServerCommand(const string &port);

    //takes values from the simulator (simulator becomes server)
    int execute(int index, vector<string> &lexer);

};

void serverThread(int client_socket);

#endif //FLIGHTSIMULATOR_OPENDATASERVERCOMMAND_H
