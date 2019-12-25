//
// Created by giladasher on 25/12/2019.
//

#include "LoopCommand.h"
#include "Data.h"

LoopCommand::LoopCommand(const string &leftStr, const string &rightStr) : ConditionParserCommand(leftStr, rightStr) {}

int LoopCommand::execute(int index, vector <string> &lexer) {
    int stepsToFirstCommand;
    stepsToFirstCommand = this->stepsToFirstCommand(index, lexer);
    int i = index + stepsToFirstCommand;
    int stepOutOfTheScope;
    stepOutOfTheScope = this->stepOutOfTheScope(index, lexer);

    while (this->checkCondition(index, lexer)) {
        while (lexer[i] != "}") {
            if (Data::getInstance()->getCommandMap().count(lexer[i]) > 0) {
                Command *command = (Data::getInstance()->getCommandMap().find(lexer[i]))->second;
                if (command != nullptr) {
                    i += command->execute(i, lexer);
                }
            }
                // assignmentCommand
            else {
                Command *assignmentCommand = Data::getInstance()->getCommandMap().find("assign")->second;
                i += assignmentCommand->execute(i, lexer);
            }
        }
        i = index + stepsToFirstCommand;
    }
    return stepOutOfTheScope;
}