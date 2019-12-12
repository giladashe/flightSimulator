#include <iostream>
#include <fstream>
#include "Lexer.h"
using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "HI Gilad";
    std::cout << "HI Israel shai halevi";
    Lexer lexer;
    ifstream filePointer;
    filePointer.open("/home/giladasher/CLionProjects/flightSimulator/fly.txt",std::ifstream::in);
    vector<string> arrayOfTokens = lexer.lexer(filePointer);
    return 0;
}
