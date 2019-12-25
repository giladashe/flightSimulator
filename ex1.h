//
// Created by shaiyis on 6.11.2019.
//

#ifndef EX1_EX1_H
#define EX1_EX1_H

#include "Expression.h"
#include <queue>
#include <stack>
#include <vector>
#include <list>

using namespace std;

//BinaryOperator
//
class BinaryOperator : public Expression {
protected:
    Expression* right;
    Expression* left;

public:
    BinaryOperator (Expression* l, Expression* r);
    virtual ~BinaryOperator() override ;
};

//Plus

class Plus : public BinaryOperator {
public:
    Plus (Expression* l, Expression* r);
    double calculate();
};

//Minus

class Minus : public BinaryOperator {
public:
    Minus (Expression* l, Expression* r);
    double calculate();
};

//Mul

class Mul : public BinaryOperator {
public:
    Mul (Expression* l, Expression* r);
    double calculate();
};

//Div

class Div : public BinaryOperator {
public:
    Div (Expression* l, Expression* r);
    double calculate();
};

// Condition
class Condition : public BinaryOperator {
    string _op;
public:
    Condition (Expression* l, Expression* r, string op);
    double calculate();
};


//UnaryOperator

class UnaryOperator : public Expression {
protected:
    Expression* expression;

public:
    UnaryOperator (Expression* e);
    virtual ~UnaryOperator() override;
};

//UPlus

class UPlus: public UnaryOperator{
public:
    UPlus (Expression* e);
    double calculate();
};

//UMinus

class UMinus: public UnaryOperator{
public:
    UMinus (Expression* e);
    double calculate();
};

class Variable : public Expression{
    string name;
    double value;
public:
    const string &getName() const;
    double getValue() const;

public:
    Variable (string name, double value);
    double calculate();
    Variable& operator++();
    Variable& operator--();
    Variable& operator+=(double add);
    Variable& operator-=(double red);
    Variable& operator++(int);
    Variable& operator--(int);

    void setValue(double v);
};

//Value

class Value : public Expression{
    double value;
public:
    Value(double v);
    double calculate();
};

class Interpreter{
    queue<string> queueSYA;
    stack<string> stackSYA;
    stack<Expression*> expStack;
    vector<Variable*> variablesVector;
    int inavlid = 0;

public:
    ~Interpreter();
    void setVariables(string s) ;
    Expression* interpret(string s);
    string replaceUnary (string s);
    void shuntingYard (string s);
    void numbersToStack();
    bool isValidNumber(const string& s);
    bool isValidVariable(const string& s);
    };

#endif //EX1_EX1_H
