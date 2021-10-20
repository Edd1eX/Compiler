//
// Created by 张瑞轩 on 2021/10/19.
//

#include "../include/symbol.h"

symbol::symbol(string name, string kind, string type) {
    this->name = name;
    this->kind = kind;
    this->type = type;
}

symbol::symbol() = default;

string symbol::toString() {
    string s = kind + " " + type + " " + name;
    int t = dim;
    while (t > 0) {
        s.append("[]");
        t--;
    }
    return s;
}

symbol::symbol(string name, string kind, string type, int params) {
    this->name = name;
    this->kind = kind;
    this->type = type;
    this->params = params;
}

void symbol::push(symbol sym) {
    param.push_back(sym);
}

symbol::symbol(int dim, string name, string kind, string type) {
    this->name = name;
    this->kind = kind;
    this->type = type;
    this->dim = dim;
}

symbolTable::symbolTable() = default;

int symbolTable::push(symbol s) {
    if (this->symbols.count(s.name) > 0) {
        return -1;
    }
    symbols[s.name] = s;
    return 0;
}

bool symbolTable::find(string s) {
    if (this->symbols.count(s) > 0) {
        return true;
    }
    if (father == nullptr) {
        return false;
    }
    return father->find(s);
}

symbol* symbolTable::get(string s)
{
    if (this->symbols.count(s) > 0) {
        return &symbols[s];
    }
    return father->get(s);
}

string symbolTable::toString() {
    string s;
    for (auto & symbol : symbols) {
        s.append(symbol.second.toString());
        if(!symbol.second.param.empty()) {
            s.append("(");
            int flag = 0;
            for(class symbol sy : symbol.second.param) {
                if (flag)
                    s.append(", ");
                s.append(sy.toString());
                flag = 1;
            }
            s.append(")");
        }
        s.append("\n");
    }
    if (father != nullptr) {
        s.append(father->toString());
    }
    return s;
}

bool symbolTable::find_on_def(string s) {
    if (this->symbols.count(s) > 0) {
        return true;
    }
    return false;
}

bool symbolTable::is_right_return() {
    if(is_return && funcType == 1 || !is_return && funcType == 2) {
        return true;
    }
    return false;
}

symbolTable::symbolTable(int funcType) {
    this->funcType = funcType;
}


symbolStack::symbolStack() {
    symbolTable table;
    symbols.push(table);
    height = 1;
}

void symbolStack::push(symbolTable s) {
    height++;
    symbols.push(s);
}

void symbolStack::pop() {
    height--;
    return symbols.pop();
}

symbolTable* symbolStack::top() {
    return &symbols.top();
}

string symbolStack::toString() {
    return symbols.top().toString();
}

void symbolStack::push_new(int funcType) {
    symbolTable table(funcType);
    table.father = &(symbols.top());
    symbols.push(table);
    height++;
}

