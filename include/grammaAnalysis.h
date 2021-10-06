//
// Created by 张瑞轩 on 2021/9/28.
//

#include "word.h"
#include "../settings/defination.h"

#ifndef COMPILER_GRAMMAANALYSIS_H
#define COMPILER_GRAMMAANALYSIS_H

void grammaAnalysis(vector<word> lexical);

int CompUnitgr(int loc);
int Declgr(int loc);
int ConstDeclgr(int loc);
int BTypegr(int loc);
int ConstDefgr(int loc);
int ConstInitValgr(int loc);
int VarDeclgr(int loc);
int VarDefgr(int loc);
int InitValgr(int loc);
int FuncDefgr(int loc);
int MainFuncDefgr(int loc);
int FuncTypegr(int loc);
int FuncFParamsgr(int loc);
int FuncFParamgr(int loc);
int Blockgr(int loc);
int BlockItemgr(int loc);
int Stmtgr(int loc);
int Expgr(int loc);
int Condgr(int loc);
int LValgr(int loc);
int PrimaryExpgr(int loc);
int Numbergr(int loc);
int UnaryExpgr(int loc);
int UnaryOpgr(int loc);
int FuncRParamsgr(int loc);
int MulExpgr(int loc);
int AddExpgr(int loc);
int RelExpgr(int loc);
int EqExpgr(int loc);
int LAndExpgr(int loc);
int LOrExpgr(int loc);
int ConstExpgr(int loc);
string get(int loc);
void error();
int nextLoc(int loc);
void printLexical(int loc);
void printGramma(string s);

#endif //COMPILER_GRAMMAANALYSIS_H
