//
// Created by shaiyis on 6.11.2019.
//
//
#include <iostream>
#include <regex>
#include "ex1.h"

using namespace std;

UnaryOperator::UnaryOperator(Expression *expression1) {
    this->expression = expression1;
}

UnaryOperator::~UnaryOperator() {
    delete this->expression;
}

UPlus::UPlus(Expression *expression1) : UnaryOperator(expression1) {}

double UPlus::calculate() {
    return this->expression->calculate();
}

UMinus::UMinus(Expression *expression1) : UnaryOperator(expression1) {}


double UMinus::calculate() {
    return -1 * this->expression->calculate();
}


BinaryOperator::BinaryOperator(Expression *leftExp, Expression *rightExp) {
    this->left = leftExp;
    this->right = rightExp;
}

BinaryOperator::~BinaryOperator() {
    delete this->right;
    delete this->left;
}

//Plus
Plus::Plus(Expression *leftExp, Expression *rightExp) : BinaryOperator(leftExp, rightExp) {}


double Plus::calculate() {
    return this->left->calculate() + this->right->calculate();
}

//Minus

Minus::Minus(Expression *leftExp, Expression *rightExp) : BinaryOperator(leftExp, rightExp) {}

double Minus::calculate() {
    return this->left->calculate() - this->right->calculate();
}

//Division

Div::Div(Expression *leftExp, Expression *rightExp) : BinaryOperator(leftExp, rightExp) {}

double Div::calculate() {
    if (this->right->calculate() == 0) {
        throw "division by zero";
    }
    return this->left->calculate() / this->right->calculate();
}

//Multiply

Mul::Mul(Expression *leftExp, Expression *rightExp) : BinaryOperator(leftExp, rightExp) {}

double Mul::calculate() {
    return this->left->calculate() * this->right->calculate();
}

//variable functions

Variable::Variable(string nameOfVar, double val) : name(nameOfVar), value(val) {}

double Variable::calculate() {
    return this->value;
}


Variable &Variable::operator++() {
    this->value++;
    return *this;
}

Variable &Variable::operator--() {
    this->value--;
    return *this;
}


Variable &Variable::operator+=(double val) {
    this->value += val;
    return *this;
}

Variable &Variable::operator-=(double val) {
    this->value -= val;
    return *this;
}

Variable &Variable::operator++(int) {
    this->value++;
    return *this;
}

Variable &Variable::operator--(int) {
    this->value--;
    return *this;
}

Value::Value(const double val) : value(val) {}

double Value::calculate() {
    return this->value;
}


//precedence for shunting yard algorithm
int Interpreter::precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '#' || op == '$')
        return 3;
    return 0;
}


Expression *Interpreter::interpret(string tokens) {
    queue<string> output;
    stack<char> ops;
    bool hadOperator = false;
    bool put_minus = false;
    int parentheses = 0;
    int i;
    //loops on all tokens and put them in the stack and queue according to Shunting yard algorithm
    for (i = 0; i < (int) tokens.length(); i++) {
        if (tokens[i] == ' ') {
            continue;
        } else if (tokens[i] == '(') {
            parentheses++;
            ops.push(tokens[i]);
            hadOperator = false;
        } else if (tokens[i] == ')') {
            if (parentheses == 0) {
                throw "Bad Input";
            }
            while (!ops.empty() && ops.top() != '(') {
                output.push(string(1, ops.top()));
                ops.pop();
            }
            if (!ops.empty() && ops.top() == '(') {
                ops.pop();
            }
            parentheses--;
            hadOperator = false;
        } else if (isdigit(tokens[i])) {
            string number = string(1, tokens[i]);
            i++;
            while (i < (int) tokens.length() && (isdigit(tokens[i]) || tokens[i] == '.')) {
                number += tokens[i];
                i++;
            }
            if (!isValidNumber(number)) {
                throw "not valid number";
            }
            output.push(number);
            i--;
            hadOperator = false;
        } else if (isOperator(tokens[i])) {
            if (hadOperator) {
                throw "illegal math expression";
            }
            if (i == 0 || tokens[i - 1] == '(') {
                if (tokens[i] == '+') {
                    //# is a symbol for UPlus
                    tokens[i] = '#';
                } else if (tokens[i] == '-') {
                    //$ is a symbol for UMinus
                    tokens[i] = '$';
                }
            }
            while (!ops.empty() && precedence(tokens[i]) <= precedence(ops.top()) && ops.top() != '(') {
                output.push(string(1, ops.top()));
                ops.pop();
            }
            ops.push(tokens[i]);
            hadOperator = true;
            //it's a variable
        } else if (isalpha(tokens[i]) || tokens[i] == '_') {
            string variable = string(1, tokens[i]);
            i++;
            while (i < (int) tokens.length() && !isParentheses(tokens[i]) && !isOperator(tokens[i])) {
                variable += tokens[i];
                i++;
            }
            i--;
            if (!isValidVariable(variable)) {
                throw "Not valid name of variable";
            }
            unordered_map<string, string>::iterator pair;

            // find value given key
            pair = this->variables.find(variable);

            //KEY NOT IN MAP
            if (pair == this->variables.end()) {
                throw "variable not found";
            }
            if (pair->second[0] == '-') {
                if (!ops.empty() && ops.top() == '$') {
                    ops.pop();
                    put_minus = true;
                }
                if (!put_minus) {
                    while (!ops.empty() && precedence('$') <= precedence(ops.top()) && ops.top() != '(') {
                        output.push(string(1, ops.top()));
                        ops.pop();
                    }
                    ops.push('$');
                }

                string num = pair->second.substr(1, pair->second.length() - 1);
                output.push(num);
                put_minus = false;
            } else {
                output.push(pair->second);
            }
            hadOperator = false;
        } else {
            throw "Bad Input";
        }
    }
    while (!ops.empty()) {
        output.push(string(1, ops.top()));
        ops.pop();
    }
    //make the expression according to the postfix string
    return this->makeExpression(output);
}

bool Interpreter::isParentheses(char token) {
    return token == '(' || token == ')';
}

bool Interpreter::isOperator(char token) {
    //# is a symbol for UPlus and $ is a symbol for UMinus
    return token == '+' || token == '-' || token == '*' || token == '/' || token == '#' || token == '$';
}

void Interpreter::setVariables(string varsAndVals) {

    string delimiterVars = ";";
    string delimiterVarVal = "=";
    list<string> splitVars = Interpreter::splitByDelimiter(varsAndVals, delimiterVars);
    //if there's a ';' at the end it's ok.. so we pop the 0 length string at the end
    if (splitVars.back().length() == 0) {
        splitVars.pop_back();
    }
    for (string splitVar : splitVars) {
        size_t equPos = splitVar.find('=');
        //check if there is a '='
        if (equPos == string::npos) //
            throw "THERE IS NO EQUAL SIGN ERROR";
        //check if '=' is at the end or at the beginning meaning there are no var/val
        if (equPos == 0 || equPos == splitVar.length() - 1) {
            throw "There is no var/val";
        }
        //check if there is more than one '='
        size_t nextEquPos = splitVar.find('=', equPos + 1);
        if (nextEquPos != string::npos)
            throw "THERE IS MORE THAN ONE EQUAL SIGN ERROR";


        list<string> varAndVal = splitByDelimiter(splitVar, delimiterVarVal);
        string var = varAndVal.front();
        if (!isValidVariable(var)) {
            throw "Not valid name of variable";
        }
        varAndVal.pop_front();
        string val = varAndVal.front();

        if (!isValidNumber(val)) {
            throw "illegal variable assignment!";
        }
        //look for the variable in the variable map
        if (!this->variables.empty()) {
            unordered_map<string, string>::iterator it;
            it = this->variables.find(var);
            if (it != this->variables.end()) {
                this->variables.at(var) = val;
                continue;
            }
        }
        this->variables.insert({var, val});
    }
}

/*
 * this function gets a postfix queue and make an expression according to it
 */
Expression *Interpreter::makeExpression(queue<string> output) {
    stack<Expression *> expressions;
    if (output.empty()) {
        throw "Bad input";
    }
    string s = output.front();
    if (!isdigit(s[0]) && s[0] != '$' && s[0] != '#') {
        throw "Bad input";
    }
    //puts all digits at the beginning of the queue in the stack as values
    while (isdigit(s[0])) {
        output.pop();
        Expression *e = new Value(stod(s));
        expressions.push(e);
        if (output.empty())
            break;
        s = output.front();
    }
    while (!output.empty()) {
        // # - UPlus , $ - UMinus
        if (isdigit(s[0])) {
            Expression *e = new Value(stod(s));
            expressions.push(e);
            //make unary expressions
        } else if (s[0] == '#' || s[0] == '$') {
            Expression *exp = expressions.top();
            expressions.pop();
            Expression *newExp;
            if (s[0] == '#') {
                newExp = new UPlus(exp);
            } else {
                newExp = new UMinus(exp);
            }
            expressions.push(newExp);
        } else {
            //make binary expressions
            Expression *e1 = expressions.top();
            expressions.pop();
            Expression *e2 = expressions.top();
            expressions.pop();
            Expression *binaryExp;
            if (s[0] == '*') {
                binaryExp = new Mul(e2, e1);
            } else if (s[0] == '+') {
                binaryExp = new Plus(e2, e1);
            } else if (s[0] == '-') {
                binaryExp = new Minus(e2, e1);
            } else {
                // it's division
                binaryExp = new Div(e2, e1);
            }
            expressions.push(binaryExp);
        }
        output.pop();
        if (!output.empty()) {
            s = output.front();
        }
    }
    Expression* result = expressions.top();
    return result;
}

// Condition
Condition::Condition(Expression *l, Expression *r, string op) : BinaryOperator(l, r) {
    this->_op = op;
}

double Condition::calculate() {
    auto left_val = this->left->calculate();
    auto right_val = this->right->calculate();
    //round the numbers so we can check
    left_val *= 1000000;
    left_val = (int) left_val;
    right_val *= 1000000;
    right_val = (int) right_val;

    if (this->_op == "!=") {
        return left_val != right_val || abs(left_val - right_val) != 0;
    } else if (this->_op == "==") {
        return left_val == right_val || abs(left_val - right_val) == 0;
    } else if (this->_op == ">=") {
        return left_val >= right_val || abs(left_val - right_val) == 0;;
    } else if (this->_op == "<=") {
        return left_val <= right_val || abs(left_val - right_val) == 0;;
    } else if (this->_op == ">") {
        return left_val > right_val;
    } else if (this->_op == "<") {
        return left_val < right_val;
    }
    return 0;
}


bool Interpreter::isValidNumber(const string &s) {
    regex numberRegex("-?[0-9]+\\.?[0-9]*");
    smatch smatch1;
    string parO("(");
    string parC(")");
    string plus("+");
    string minus("-");
    string uPlus("#");
    string uMinus("%");
    string mult("*");
    string div("/");
    return regex_match(s, smatch1, numberRegex);
}

bool Interpreter::isValidVariable(const string &s) {
    regex variableRegex("[a-z|A-Z|_]+[a-z|A-Z|_|0-9]*");
    smatch smatch1;
    string parO("(");
    string parC(")");
    string plus("+");
    string minus("-");
    string uPlus("#");
    string uMinus("%");
    string mult("*");
    string div("/");
    bool isValidVariable = regex_match(s, smatch1, variableRegex);
    return isValidVariable;
}

//split a string by delimiter
list<string> Interpreter::splitByDelimiter(string &s, const string& delimiter) {
    //from https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    size_t pos = 0;
    string token;
    list<string> splitString;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
        splitString.push_back(token);
    }
    splitString.push_back(s);
    return splitString;
}