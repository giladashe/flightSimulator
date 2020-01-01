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
#include <unordered_map>

using namespace std;





class UnaryOperator : public Expression {
protected:
    Expression *expression;
public:

    UnaryOperator(Expression *expression1);

    virtual ~UnaryOperator();
};

class UPlus : public UnaryOperator {
public:
    UPlus(Expression *expression1);

public:
    double calculate() override;
};


class UMinus : public UnaryOperator {
public:
    UMinus(Expression *expression);

    double calculate() override;
};


class BinaryOperator : public Expression {
protected:
    Expression *left;
    Expression *right;
public:
    BinaryOperator(Expression *leftExp, Expression *rightExp);

    virtual ~BinaryOperator() override;

};

class Plus : public BinaryOperator {
public:
    Plus(Expression *leftExp, Expression *rightExp);


    double calculate() override;
};

class Minus : public BinaryOperator {
public:
    Minus(Expression *leftExp, Expression *rightExp);

    double calculate() override;
};

class Div : public BinaryOperator {
public:

    Div(Expression *leftExp, Expression *rightExp);

    double calculate() override;
};

class Mul : public BinaryOperator {
public:
    Mul(Expression *leftExp, Expression *rightExp);

    double calculate() override;
};

// Condition
class Condition : public BinaryOperator {
    string _op;
public:
    Condition(Expression *l, Expression *r, string op);

    double calculate() override ;
};

class Variable : public Expression {
    string name;
    double value;
public:

    Variable(string nameOfVar, double val);

    double calculate() override;

    Variable &operator++();

    Variable &operator--();

    Variable &operator+=(double val);

    Variable &operator-=(double val);

    Variable &operator++(int);

    Variable &operator--(int);

};

class Value : public Expression {
    const double value;
public:
    Value(double val);

    double calculate() override;
};

class Interpreter {
    unordered_map<string, string> variables;
public:
    Expression *interpret(string tokens);

    int precedence(char op);

    bool isParentheses(char token);

    bool isOperator(char token);

    void setVariables(string varsAndVals);

    Expression *makeExpression(queue<string> output);

    bool isValidVariable(const string &s);

    bool isValidNumber(const string &s);

    static list<string> splitByDelimiter(string &s, const string& delimiter);
};

#endif //EX1_EX1_H
