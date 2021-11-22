//
// Created by 张瑞轩 on 2021/11/9.
//

#include <bits/stdc++.h>
using namespace std;

#ifndef COMPILER_INSTR_H
#define COMPILER_INSTR_H

class instr {
public:
    string name = "";
    int x = 0;
    string y = "";
    // 0 —— 指令
    // 1 —— 标签

    int flag = 0;

    instr();
    instr(string name);
    instr(string name, int x);
    instr(string name, string y);
    instr(string name, int x, string y);
    instr(int flag ,string name);
    string tostring();
};


#endif //COMPILER_INSTR_H
