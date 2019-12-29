//
// Created by giladasher on 25/12/2019.
//

#include "DefineVarCommand.h"
#include "Data.h"
#include <algorithm>

DefineVarCommand::DefineVarCommand(const string &var, const string &value) : _var(var), _value(value) {}

int DefineVarCommand::execute(int index, vector<string> &lexer) {
    auto data = Data::getInstance();
    // if it's new Var definition
    if ((lexer[index + 2] == "->") || (lexer[index + 2] == "<-")) {
        string progStr = lexer[index + 1];
        string simStr = lexer[index + 4];
        simStr.erase(remove(simStr.begin(), simStr.end(), '"'), simStr.end());

        // bindOfVar = 1  - program affects simulator
        // bindOfVar = 0  - simulator affects program

        int bindOfVar;
        if (lexer[index + 2] == "->") {
            bindOfVar = 1;
        } else {
            bindOfVar = 0;
        }

        // create VarData obj and insert to progMap
        auto *varData = new VarData(0, progStr, simStr, bindOfVar);
        data->setProgMap(progStr, varData);
        if (bindOfVar == 0) {
            data->updateBindSimMap(simStr, 1);
            data->setProgStrSimMap(simStr, progStr);
        }

        return 6;
    }

    // assignment  (var a = b)
    else {
        string progStr = lexer[index + 1];
        string simStr = "";

        // create VarData obj and insert to progMap
        auto *varData = new VarData(0, progStr, simStr, 0);

        // insert to progMap
        data->setProgMap(progStr, varData);

        // getting assignment command
        Command *assignmentCommand = (Command*)data->getCommandMap("assign");

        // steps out
        int toJump = assignmentCommand->execute(index + 1, lexer);
        return toJump + 1;
    }
}