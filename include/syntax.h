//
// Created by 张瑞轩 on 2021/10/16.
//
#include "word.h"
#include "symbol.h"
#include "../settings/defination.h"
#ifndef COMPILER_SYNTAX_H
#define COMPILER_SYNTAX_H

class syntax {
public:
    string s = "";
    string category = "";
    int line;
    int dim;
    /*
     * flag:
     * 0  —— 终结符
     * 1  —— 非终结符
     * -1 —— error
     */
    int flag = -1;
    string syn_type = "";
    vector<syntax> child;

    syntax(word w);
    syntax(string syn_type);
    syntax(int error);

    void insert(syntax c);
    bool iserror();
    string getName();
    string getType();
    bool checkLast();
    int count_params();
    vector<symbol> get_param();
};


#endif //COMPILER_SYNTAX_H
