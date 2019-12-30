//
// Created by giladasher on 25/12/2019.
//

#include <iostream>
#include "PrintCommand.h"
#include "Lexer.h"
#include "Data.h"

PrintCommand::PrintCommand(const string &str) : _str(str) {}

int PrintCommand::execute(int index, vector <string> &lexer) {
    //checks if there are " in the expression and if there are prints the message without them
    string message = string(lexer[index + 1]);
    vector <string> theMessageWithout = Lexer::splitByDelimiter(message, "\"");
    if (theMessageWithout.size() > 1) {
        message = theMessageWithout[1];
    } else {
        //if it's an expression that it calculates it and then prints it
        auto data = Data::getInstance();
        Data::updateVariablesFromStr(message);
        Expression* e = data->getInterpreter()->interpret(message);
        double value = e->calculate();
        delete e;
        message = to_string(value);
    }
    cout << message << endl;
    return 3;
}