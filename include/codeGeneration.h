//
// Created by 张瑞轩 on 2021/11/10.
//

#ifndef COMPILER_CODEGENERATION_H
#define COMPILER_CODEGENERATION_H
#include "syntaxAnalysis.h"
#include "interpreter.h"

void generation();

void CompUnitge(syntax node);
void Declge(syntax node);
void ConstDeclge(syntax node);
void BTypege(syntax node);
void ConstDefge(syntax node);
void ConstInitValge(syntax node, string name);
void VarDeclge(syntax node);
void VarDefge(syntax node);
void InitValge(syntax node, string name);
void FuncDefge(syntax node);
void MainFuncDefge(syntax node);
void FuncTypege(syntax node);
void FuncFParamsge(syntax node);
void FuncFParamge(syntax node);
void Blockge(syntax node);
void BlockItemge(syntax node);
void Stmtge(syntax node);
void Expge(syntax node);
void Condge(syntax node);
void LValge(syntax node);
void LValAssign(syntax node);
void PrimaryExpge(syntax node);
void Numberge(syntax node);
void UnaryExpge(syntax node);
void UnaryOpge(syntax node);
void FuncRParamsge(syntax node);
void MulExpge(syntax node);
void AddExpge(syntax node);
void RelExpge(syntax node);
void EqExpge(syntax node);
void LAndExpge(syntax node);
void LOrExpge(syntax node);
void ConstExpge(syntax node);
void createInstr(string name, int x);
void createInstr(string name, string y);
void createInstr(int flag, string name);
void createInstr(string name, int x, string y);
void createInstr(string name);
string make_if_label(int flag);
string make_while_label(int flag);
string make_and_or_label(int x);

#endif //COMPILER_CODEGENERATION_H
