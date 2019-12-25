//
// Created by giladasher on 25/12/2019.
//

#include "MakeFuncCommand.h"
#include "FuncCommand.h"
#include "Data.h"

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
    Data::getInstance()->m.lock();
    Data::getInstance()->setCommandMap(lexer[index], funcCommand);
    Data::getInstance()->m.unlock();

    return i;
}