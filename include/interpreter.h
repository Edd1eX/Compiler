//
// Created by 张瑞轩 on 2021/11/9.
//


#include "instr.h"
#include "memory.h"
#include "../settings/defination.h"
using namespace std;

#ifndef COMPILER_INTERPRETER_H
#define COMPILER_INTERPRETER_H

void debug();
void run(vector<instr>);
void push(int x);
void pop();
void add();
void sub();
void mul();
void div();
void mod();
void lss();
void leq();
void eq();
void neq();
void gt();
void gte();
void read();
void print();
void print(string s);
void init(string s);
void init2(string s);
void storei(string s);
void ldi(string s);
void jump(string s);
void jal(string s);
void copy();
void bez(string s);
void bnz(string s);
void jback();
void cnb();
void dnb();
void cb();
void db();
void land();
void lor();
void ops();
void no();
void clear(int flag);
void off(string s, int x);
void off2(string s, int div);

#endif //COMPILER_INTERPRETER_H
