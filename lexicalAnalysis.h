//
// Created by 张瑞轩 on 2021/9/23.
//

#include "word.h"
#include "defination.cpp"
using namespace std;

#ifndef COMPILER_LEXICALANALYSIS_H
#define COMPILER_LEXICALANALYSIS_H

void analysis();
void output();
void getText();
void go();

int INTCONr(int loc);
int STRCONr(int loc);
int NOTr(int loc);
int ANDr(int loc);
int ORr(int loc);
int PLUSr(int loc);
int MINUr(int loc);
int MULTr(int loc);
int DIVr(int loc);
int MODr(int loc);
int LSSr(int loc);
int LEQr(int loc);
int GREr(int loc);
int GEQr(int loc);
int EQLr(int loc);
int NEQr(int loc);
int ASSIGNr(int loc);
int SEMICNr(int loc);
int COMMAr(int loc);
int LPARENTr(int loc);
int RPARENTr(int loc);
int LBRACKr(int loc);
int RBRACKr(int loc);
int LBRACEr(int loc);
int RBRACEr(int loc);
int WORDr(int loc);
int skipComment1(int loc);
int skipComment2(int loc);

#endif //COMPILER_LEXICALANALYSIS_H
