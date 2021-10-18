//
// Created by 张瑞轩 on 2021/10/16.
//
#include "word.h"
#ifndef COMPILER_SYNTAX_H
#define COMPILER_SYNTAX_H


class syntax {
public:
    string s;
    string category;
    int line;
    /*
     * flag:
     * 0  —— 终结符
     * 1  —— 非终结符
     * -1 —— error
     */
    int flag;
    string syn_type;
    vector<syntax> child;

    syntax(word w);
    syntax(string syn_type);
    syntax(int error);

    void insert(syntax c);
    bool iserror();
};


#endif //COMPILER_SYNTAX_H
