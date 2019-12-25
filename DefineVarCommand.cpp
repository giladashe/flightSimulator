//
// Created by giladasher on 25/12/2019.
//

#include "DefineVarCommand.h"
#include "Data.h"
#include <algorithm>

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
        Data::getInstance()->m.lock();
        auto *varData = new VarData(0, progStr, simStr, bindOfVar);
        Data::getInstance()->setProgMap(progStr, varData);
        if (bindOfVar == 0) {
            Data::getInstance()->updateBindSimMap(simStr, 1);
        }
        Data::getInstance()->m.unlock();
        return 6;
    }

        // assignment
    else {
        string progStr = lexer[index + 1];
        string simStr;
        int bind = 0;
        // create VarData obj and insert to progMap
        auto *varData = new VarData(0, progStr, simStr, bind);
        Data::getInstance()->m.lock();
        Data::getInstance()->setProgMap(progStr, varData);

        Command *assignmentCommand = Data::getInstance()->getCommandMap().find("assign")->second;
        Data::getInstance()->m.unlock();
        int toJump = assignmentCommand->execute(index + 1, lexer);
        return toJump + 1;
    }
}