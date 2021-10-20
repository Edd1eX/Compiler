//
// Created by 张瑞轩 on 2021/10/19.
//
#include "word.h"
#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H


class symbol {
public:
    string name;
    string kind;
    string type;
    int dim = 0;
    int params = 0;
    vector<symbol> param;

    symbol();
    symbol(string name, string kind, string type);
    symbol(string name, string kind, string type, int params);
    symbol(int dim, string name, string kind, string type);
    void push(symbol sym);
    string toString();
};

class symbolTable {
public:
    map<string, symbol> symbols;
    symbolTable *father = nullptr;
    // 0 —— none
    // 1 —— int
    // 2 —— void
    int funcType = 0;
    int params = 0;
    bool is_return = false;

    symbolTable();
    symbolTable(int funcType);

    int push(symbol s);
    bool find(string s);
    bool find_on_def(string s);
    bool is_right_return();
    symbol * get(string s);
    string toString();
};

class symbolStack {
public:
    stack<symbolTable> symbols;
    int height = 0;

    symbolStack();
    void push(symbolTable s);
    void push_new(int funcType);
    void pop();
    symbolTable* top();
    string toString();
};
#endif //COMPILER_SYMBOL_H
