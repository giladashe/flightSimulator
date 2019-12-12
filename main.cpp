#include <iostream>
#include <fstream>
#include "Lexer.h"
using namespace std;
int main() {
    Lexer lexer;
    ifstream filePointer;
    filePointer.open("/home/giladasher/CLionProjects/flightSimulator/fly.txt",std::ifstream::in);
    vector<string> arrayOfTokens = lexer.lexer(filePointer);
    return 0;
}
