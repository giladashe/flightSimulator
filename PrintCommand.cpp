//
// Created by giladasher on 25/12/2019.
//

#include <iostream>
#include "PrintCommand.h"
#include "Lexer.h"
#include "Data.h"

PrintCommand::PrintCommand(const string &str) : str(str) {}

int PrintCommand::execute(int index, vector <string> &lexer) {
    string message = string(lexer[index + 1]);
    vector <string> theMessageWithout = Lexer::splitByDelimiter(message, "\"");
    if (theMessageWithout.size() > 1) {
        message = theMessageWithout[1];
    } else {
          Data::getInstance()->progMapMutex.lock();
        double value = Data::getInstance()->getProgMap()[message]->getValue();
          Data::getInstance()->progMapMutex.unlock();
        message = to_string(value);
    }
    cout << message << endl;
    return 3;
}