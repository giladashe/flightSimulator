#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Lexer.h"
#include "VarData.cpp"
#include "Command.cpp"

using namespace std;
int main(){
    Lexer lexer;
    ifstream filePointer;
    filePointer.open("/home/shaiyis/CLionProjects/flightSimulator/fly.txt",std::ifstream::in);
    vector<string> arrayOfTokens = lexer.lexer(filePointer);

    unordered_map<string, VarData> clientMap;
    unordered_map<string, VarData> simMap; //TODO think where to put

    return 0;
}
