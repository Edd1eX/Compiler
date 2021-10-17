//
// Created by 张瑞轩 on 2021/10/16.
//

#include "word.h"
#include "syntax.h"
#include "../settings/defination.h"

#ifndef COMPILER_SYNTAXANALYSIS_H
#define COMPILER_SYNTAXANALYSIS_H

void syntaxAnalysis(vector<word> lexical);

syntax CompUnitgr();
syntax Declgr();
syntax ConstDeclgr();
syntax BTypegr();
syntax ConstDefgr();
syntax ConstInitValgr();
syntax VarDeclgr();
syntax VarDefgr();
syntax InitValgr();
syntax FuncDefgr();
syntax MainFuncDefgr();
syntax FuncTypegr();
syntax FuncFParamsgr();
syntax FuncFParamgr();
syntax Blockgr();
syntax BlockItemgr();
syntax Stmtgr();
syntax Expgr();
syntax Condgr();
syntax LValgr();
syntax PrimaryExpgr();
syntax Numbergr();
syntax UnaryExpgr();
syntax UnaryOpgr();
syntax FuncRParamsgr();
syntax MulExpgr();
syntax AddExpgr();
syntax RelExpgr();
syntax EqExpgr();
syntax LAndExpgr();
syntax LOrExpgr();
syntax ConstExpgr();
string get(int loc);
syntax error();
syntax get_next();
void output(syntax s);

#endif //COMPILER_SYNTAXANALYSIS_H
