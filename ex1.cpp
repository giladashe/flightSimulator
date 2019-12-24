//
// Created by shaiyis on 6.11.2019.
//

#include <iostream>
#include <cstring>
#include <regex>
#include <exception>
#include "ex1.h"

using namespace std;

//BinaryOperator

BinaryOperator::BinaryOperator(Expression *l, Expression *r) {
    this->left = l;
    this->right = r;
}

BinaryOperator::~BinaryOperator() {
    delete this->left;
    delete this->right;
}

//Plus

Plus::Plus(Expression *l, Expression *r) : BinaryOperator(l, r) {
}

double Plus::calculate() {
    return left->calculate() + right->calculate();
}

//Minus

Minus::Minus(Expression *l, Expression *r) : BinaryOperator(l, r) {
}

double Minus::calculate() {
    return left->calculate() - right->calculate();
}

//Mul

Mul::Mul(Expression *l, Expression *r) : BinaryOperator(l, r) {
}

double Mul::calculate() {
    return left->calculate() * right->calculate();
}

//Div

Div::Div(Expression *l, Expression *r) : BinaryOperator(l, r) {
}

double Div::calculate() {
    double l = right->calculate();
    if (l == 0) {
        throw "You mustn't divide by 0!";
    }
    return left->calculate() / right->calculate();
}

// Condition
Condition::Condition(Expression *l, Expression *r, string op) : BinaryOperator(l, r) {
    this->_op = op;
}

double Condition::calculate() {
    if (this->_op == "!=") {
        return this->left != this->right;
    } else if (this->_op == "==") {
        return this->left == this->right;
    } else if (this->_op == ">=") {
        return this->left >= this->right;
    } else if (this->_op == "<=") {
        return this->left <= this->right;
    } else if (this->_op == ">") {
        return this->left > this->right;
    } else if (this->_op == "<") {
        return this->left < this->right;
    }
}



//UnaryOperator

UnaryOperator::UnaryOperator(Expression *e) {
    this->expression = e;
}

UnaryOperator::~UnaryOperator() {
    delete this->expression;
}

//UPlus

UPlus::UPlus(Expression *e) : UnaryOperator(e) {
}

double UPlus::calculate() {
    return expression->calculate();
}

//UMinus

UMinus::UMinus(Expression *e) : UnaryOperator(e) {
}

double UMinus::calculate() {
    return -expression->calculate();
}

//Variable

Variable::Variable(string n, double v) {
    this->name = n;
    this->value = v;
}

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

Variable &Variable::operator+=(double add) {
    this->value += add;
    return *this;
}

Variable &Variable::operator-=(double red) {
    this->value -= red;
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

const string &Variable::getName() const {
    return name;
}

double Variable::getValue() const {
    return value;
}

void Variable::setValue(double v) {
    Variable::value = v;
}


//Value

Value::Value(double v) {
    this->value = v;
}

double Value::calculate() {
    return this->value;
}


Interpreter::~Interpreter() {
    for (auto i = this->variablesVector.begin(); i != this->variablesVector.end(); ++i) {
        delete *i;
    }
}

void Interpreter::setVariables(string s) {

    // split by ";"

    vector<string> v;
    size_t pos = 0;
    string delimiter = ";";
    string delimiter1 = "=";
    string tokenString;
    int k = 0;

    if ((s.find(delimiter) == string::npos) && (s.find(delimiter1) == string::npos)) {
        throw "bad input";
    }

    while ((pos = s.find(delimiter)) != string::npos) {
        tokenString = s.substr(0, pos);
        v.push_back(tokenString);
        s.erase(0, pos + 1);
    }
    v.push_back(s);

    // split by "="

    string delimiter2 = "=";

    for (auto i = v.begin(); i != v.end(); ++i) {

        // case of ";" at the end
        if ((*i).compare("") == 0) {
            break;
        }
        string name = (*i).substr(0, (*i).find(delimiter2));
        if (!isValidVariable(name)) {
            throw "Invalid variable name";
        }
        if ((name.find("=") != string::npos) ||
            ((*i).substr((*i).find(delimiter2) + 1, string::npos).find("=") != string::npos)) {
            throw "Invalid input";
        }

        // if the value has a minus

        string val = (*i).substr((*i).find(delimiter2) + 1, string::npos);
        if (val.substr(0, 1).compare("-") == 0) {
            if (!isValidNumber(val.substr(1, string::npos))) {
                throw "Invalid variable value";
            }
        } else {
            if (!isValidNumber(val)) {
                throw "Invalid variable value";
            }
        }


        double value = stod((*i).substr((*i).find(delimiter2) + 1, string::npos));
        for (Variable *ve: this->variablesVector) {
            if (ve->getName().compare(name) == 0) {
                ve->setValue(value);
                k = 1;
                break;
            }
        }
        if (k == 0) {
            Variable *variable = new Variable(name, value);
            this->variablesVector.push_back(variable);
        }
    }
}


//from string queue and stack to expression stack
Expression *Interpreter::interpret(const string str) {

    // if the string contains ++/--/**/ //

    if ((str.find("--") != string::npos) || (str.find("++") != string::npos) ||
        (str.find("**") != string::npos) || (str.find("//") != string::npos)) {
        throw "Invalid input.";
    }

    string replacedStr = replaceUnary(str);
    shuntingYard(replacedStr);
    string plus("+");
    string minus("-");
    string uPlus("#");
    string uMinus("%");
    string mult("*");
    string div("/");
    string parO("(");
    string parC(")");

    while (!this->queueSYA.empty()) {

        //  if there is +, -, *, / in front of the queue
        if ((this->queueSYA.front() == plus) ||
            (this->queueSYA.front() == minus) ||
            (this->queueSYA.front() == mult) ||
            (this->queueSYA.front() == div)) {


            //taking 2 arguments: both from expStack
            if (this->stackSYA.empty() && (this->expStack.size() >= 2)) {
                Expression *expression1 = this->expStack.top();
                this->expStack.pop();
                Expression *expression2 = this->expStack.top();
                this->expStack.pop();

                if (this->queueSYA.front() == plus) {
                    Expression *pl = new Plus(expression2, expression1);
                    this->expStack.push(pl);
                }
                if (this->queueSYA.front() == minus) {
                    Expression *min = new Minus(expression2, expression1);
                    this->expStack.push(min);
                }
                if (this->queueSYA.front() == mult) {
                    Expression *m = new Mul(expression2, expression1);
                    this->expStack.push(m);
                }
                if (this->queueSYA.front() == div) {
                    Expression *d = new Div(expression2, expression1);
                    this->expStack.push(d);
                }
                this->queueSYA.pop();

                numbersToStack();
            }

            //  if there is Uplus or Uminus in front of the queue
        } else if ((this->queueSYA.front() == uPlus) ||
                   (this->queueSYA.front() == uMinus)) {

            //taking one argument from expStack
            if (!this->expStack.empty()) {
                Expression *expression = this->expStack.top();
                this->expStack.pop();

                if (this->queueSYA.front() == uPlus) {
                    Expression *uP = new UPlus(expression);
                    this->expStack.push(uP);
                    this->queueSYA.pop();
                    numbersToStack();
                } else if (this->queueSYA.front() == uMinus) {
                    Expression *uM = new UMinus(expression);
                    this->expStack.push(uM);
                    this->queueSYA.pop();
                    numbersToStack();
                }
            }
        }
    }
    Expression *expression = this->expStack.top();
    return expression;
}

bool Interpreter::isValidNumber(const string& s) {
    regex numberRegex("[0-9]+\\.?[0-9]*");
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

bool Interpreter::isValidVariable(const string& s) {
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

void Interpreter::shuntingYard(string replacedStr) {
    regex variableRegex("[a-z|A-Z|_]+[a-z|A-Z|_|0-9]*");
    regex numberRegex("[0-9]+\\.?[0-9]*");
    smatch smatch1;
    string parO("(");
    string parC(")");
    string plus("+");
    string minus("-");
    string uPlus("#");
    string uMinus("%");
    string mult("*");
    string div("/");

    // curr is one char from above, sub is the subSting until curr

    for (unsigned long i = 0; i < replacedStr.size() + 1; i++) {
        i = 0;
        int j = 0;
        string curr = replacedStr.substr(0, 1);

        // if there is a space
        if (curr == " ") {
            throw "No spaces in input";
        }

        while ((curr != parO) && (curr != parC) && (curr.compare(plus) != 0) &&
               (curr.compare(minus) != 0) && (curr.compare(uPlus) != 0) && (curr.compare(uMinus) != 0) &&
               (curr.compare(mult) != 0) && (curr.compare(div) != 0)) {
            if (replacedStr.size() == 1) {
                break;
            }
            if (curr.compare("") == 0) {
                break;
            }
            j++;
            curr = replacedStr.substr(j, 1);

            // if there is a space
            if (curr.compare(" ") == 0) {
                throw "No spaces in input";
            }

        }

        string sub = replacedStr.substr(0, j);
        int found = 0;

        // numbers to queue
        if (sub.length() > 0) {
            if (regex_match(sub, smatch1, variableRegex)) {

                // if it's a variable, search for its value
                for (Variable *v: variablesVector) {
                    //smatch1.str(0)
                    if (sub.compare(v->getName()) == 0) {
                        this->queueSYA.push(to_string(v->getValue()));
                        found = 1;
                    }
                }
                if (found == 0) {
                    throw "Variable have not found.";
                }
            } else if (regex_match(sub, smatch1, numberRegex)) {
                this->queueSYA.push(sub);
            }

                // if not a  valid number or value
            else {
                this->inavlid = 1;
            }
        }
        if (this->inavlid == 1) {
            throw "Inavlid Input";
        }

        if (curr.compare("") == 0) {
            break;
        }

        // operators to stack by precedence

        if (curr.compare(parO) == 0) {
            this->stackSYA.push(parO);
        } else if (curr.compare(parC) == 0) {
            // if no "(" for ")" was found
            if (this->stackSYA.empty()) {
                throw "Invaild input";
            }

            while ((!this->stackSYA.empty()) && (this->stackSYA.top().compare(parO) != 0)) {
                this->queueSYA.push(this->stackSYA.top());
                this->stackSYA.pop();

                // if no "(" for ")" was found
                if (this->stackSYA.empty()) {
                    throw "Invaild input";
                }
            }
            this->stackSYA.pop();
        } else if (curr.compare(plus) == 0) {
            while ((!this->stackSYA.empty()) && ((this->stackSYA.top().compare(uPlus) == 0) ||
                                                 (this->stackSYA.top().compare(uMinus) == 0) ||
                                                 (this->stackSYA.top().compare(mult) == 0) ||
                                                 (this->stackSYA.top().compare(div) == 0))) {
                this->queueSYA.push(this->stackSYA.top());
                this->stackSYA.pop();
            }
            this->stackSYA.push(plus);
        } else if (curr.compare(minus) == 0) {
            while ((!this->stackSYA.empty()) && ((this->stackSYA.top().compare(uPlus) == 0) ||
                                                 (this->stackSYA.top().compare(uMinus) == 0) ||
                                                 (this->stackSYA.top().compare(mult) == 0) ||
                                                 (this->stackSYA.top().compare(div) == 0))) {
                this->queueSYA.push(this->stackSYA.top());
                this->stackSYA.pop();
            }
            this->stackSYA.push(minus);
        } else if (curr.compare(uPlus) == 0) {
            this->stackSYA.push(uPlus);
        } else if (curr.compare(uMinus) == 0) {
            this->stackSYA.push(uMinus);
        } else if (curr.compare(mult) == 0) {
            while ((!this->stackSYA.empty()) && ((this->stackSYA.top().compare(uPlus) == 0) ||
                                                 (this->stackSYA.top().compare(uMinus) == 0))) {
                this->queueSYA.push(this->stackSYA.top());
                this->stackSYA.pop();
            }
            this->stackSYA.push(mult);
        } else if (curr.compare(div) == 0) {
            while ((!this->stackSYA.empty()) && ((this->stackSYA.top().compare(uPlus) == 0) ||
                                                 (this->stackSYA.top().compare(uMinus) == 0))) {
                this->queueSYA.push(this->stackSYA.top());
                this->stackSYA.pop();
            }
            this->stackSYA.push(div);
        }

        // if the last char is a number
        if (regex_match(curr, smatch1, variableRegex)) {

            // if it's a variable, search for its value
            for (Variable *v: variablesVector) {
                //smatch1.str(0)
                if (curr.compare(v->getName()) == 0) {
                    this->queueSYA.push(to_string(v->getValue()));
                    found = 1;
                }
            }
            if (found == 0) {
                throw "Variable have not found.";
            }
        }

        if (regex_match(curr, smatch1, numberRegex)) {
            this->queueSYA.push(curr);
        }

        replacedStr.erase(0, j + 1);
    }


    // finally, stack to queue

    while (!this->stackSYA.empty()) {

        // if there are more ( for every )
        if (this->stackSYA.top().compare("(") == 0) {
            throw "Invalid input";
        }
        this->queueSYA.push(this->stackSYA.top());
        this->stackSYA.pop();
    }

    numbersToStack();
}


void Interpreter::numbersToStack() {

    string plus("+");
    string minus("-");
    string uPlus("#");
    string uMinus("%");
    string mult("*");
    string div("/");
    string parO("(");
    string parC(")");

    while ((!this->queueSYA.empty()) && ((this->queueSYA.front().compare(parO) != 0) &&
                                         (this->queueSYA.front().compare(parC) != 0) &&
                                         (this->queueSYA.front().compare(plus) != 0) &&
                                         (this->queueSYA.front().compare(minus) != 0) &&
                                         (this->queueSYA.front().compare(uPlus) != 0) &&
                                         (this->queueSYA.front().compare(uMinus) != 0) &&
                                         (this->queueSYA.front().compare(mult) != 0) &&
                                         (this->queueSYA.front().compare(div) != 0))) {
        Expression *v = new Value(stod(this->queueSYA.front()));
        this->expStack.push(v);
        this->queueSYA.pop();
    }
}


string Interpreter::replaceUnary(string str) {

    regex variableRegexBegin("[a-z|A-Z|_]");
    smatch smatch2;

    // Replace Unary + by "#", Unary - by "%"
    string replacedStr = str;
    if (str.rfind("+", 0) == 0) {
        replacedStr.replace(0, 1, "#");
    } else if (str.rfind("-", 0) == 0) {
        replacedStr.replace(0, 1, "%");

        // if there is a variable after % at the beginning
        string afterUminus1 = str.substr(1, 2);
        if (regex_match(afterUminus1, smatch2, variableRegexBegin)) {
            throw "Invalid Input";
        }
    }


    for (unsigned long i = 1; i < replacedStr.size(); ++i) {
        string curr = replacedStr.substr(i, 1);
        string prev = replacedStr.substr(i - 1, 1);
        string s2("(");
        string s3("+");
        string s4("-");
        string s5("*");
        string s6("/");

        if ((curr.compare("+") == 0) &&
            ((prev.compare(s2) == 0) || (prev.compare(s3) == 0) || (prev.compare(s4) == 0) ||
             (prev.compare(s5) == 0) || (prev.compare(s6) == 0))) {
            replacedStr.replace(i, 1, "#");
        } else if (((curr.compare("-")) == 0) &&
                   ((prev.compare(s2) == 0) || (prev.compare(s3) == 0) || (prev.compare(s4) == 0) ||
                    (prev.compare(s5) == 0) || (prev.compare(s6) == 0))) {
            replacedStr.replace(i, 1, "%");

            // if there is a variable after %
            string afterUminus2 = str.substr(i + 1, 1);
            if (regex_match(afterUminus2, smatch2, variableRegexBegin)) {
                throw "Invalid Input";
            }
        }
    }
    return replacedStr;
}



