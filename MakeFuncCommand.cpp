//
// Created by giladasher on 25/12/2019.
//

#include "MakeFuncCommand.h"
#include "FuncCommand.h"
#include "Data.h"

int MakeFuncCommand::execute(int index, vector<string> &lexer) {
    int count = 0;
    int i = 0;
    i = 3;
    int j = index;
    auto *funcCommand = new FuncCommand(lexer[j + 3]);
    i += 4;
    j += i;
    funcCommand->setStartIndex(j);
    //start on the first command
    while (true) {
        if (lexer[j] == "{") {
            count++;
        } else if (lexer[j] == "}") {
            if (count == 0) {
                break;
            } else {
                count--;
            }
        }
        j++;
        i++;
    }
    funcCommand->setEndIndex(j);
    Data::getInstance()->setCommandMap(lexer[index], funcCommand);


    return i+2;
}