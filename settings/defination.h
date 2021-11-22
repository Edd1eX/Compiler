//
// Created by 张瑞轩 on 2021/10/5.
//

#ifndef COMPILER_DEFINATION_H
#define COMPILER_DEFINATION_H

const string IDENFR = "IDENFR";     // 变量
const string INTCON = "INTCON";     // 整数
const string STRCON = "STRCON";     // 字符串
const string MAINTK = "MAINTK";     // main
const string CONSTTK = "CONSTTK";   // const
const string INTTK = "INTTK";       // int
const string BREAKTK = "BREAKTK";   // break
const string CONTINUETK = "CONTINUETK"; // continue
const string IFTK = "IFTK";         // if
const string ELSETK = "ELSETK";     // else
const string NOT = "NOT";           // !
const string AND = "AND";           // &&
const string OR = "OR";             // ||
const string WHILETK = "WHILETK";   // while
const string GETINTTK = "GETINTTK"; // getint
const string PRINTFTK = "PRINTFTK"; // printf
const string RETURNTK = "RETURNTK"; // return
const string PLUS = "PLUS";         // +
const string MINU = "MINU";         // -
const string VOIDTK = "VOIDTK";     // void
const string MULT = "MULT";         // *
const string DIV = "DIV";           // /
const string MOD = "MOD";           // %
const string LSS = "LSS";           // <
const string LEQ = "LEQ";           // <=
const string GRE = "GRE";           // >
const string GEQ = "GEQ";           // >=
const string EQL = "EQL";           // ==
const string NEQ = "NEQ";           // !=
const string ASSIGN = "ASSIGN";     // =
const string SEMICN = "SEMICN";     // ;
const string COMMA = "COMMA";       // ,
const string LPARENT = "LPARENT";   // (
const string RPARENT = "RPARENT";   // )
const string LBRACK = "LBRACK";     // [
const string RBRACK = "RBRACK";     // ]
const string LBRACE = "LBRACE";     // {
const string RBRACE = "RBRACE";     // }

const string CompUnit = "<CompUnit>";
const string Decl = "<Decl>";
const string ConstDecl = "<ConstDecl>";
const string BType = "<BType>";
const string ConstDef = "<ConstDef>";
const string ConstInitVal = "<ConstInitVal>";
const string VarDecl = "<VarDecl>";
const string VarDef = "<VarDef>";
const string InitVal = "<InitVal>";
const string FuncDef = "<FuncDef>";
const string MainFuncDef = "<MainFuncDef>";
const string FuncType = "<FuncType>";
const string FuncFParams = "<FuncFParams>";
const string FuncFParam = "<FuncFParam>";
const string Block = "<Block>";
const string BlockItem = "<BlockItem>";
const string Stmt = "<Stmt>";
const string Exp = "<Exp>";
const string Cond = "<Cond>";
const string LVal = "<LVal>";
const string PrimaryExp = "<PrimaryExp>";
const string Number = "<Number>";
const string UnaryExp = "<UnaryExp>";
const string UnaryOp = "<UnaryOp>";
const string FuncRParams = "<FuncRParams>";
const string MulExp = "<MulExp>";
const string AddExp = "<AddExp>";
const string RelExp = "<RelExp>";
const string EqExp = "<EqExp>";
const string LAndExp = "<LAndExp>";
const string LOrExp = "<LOrExp>";
const string ConstExp = "<ConstExp>";

// instruction
const string Iadd = "add";
const string Ibez = "bez";
const string Ibnz = "bnz";
const string Icb = "cb";
const string Icnb = "cnb";
const string Icopy = "copy";
const string Idb = "db";
const string Idiv = "div";
const string Idnb = "dnb";
const string Iend = "end";
const string Ieq = "eq";
const string Igt = "gt";
const string Igte = "gte";
const string Iinit = "init";
const string Iinit2 = "init2";
const string Ijal = "jal";
const string Ijback = "jback";
const string Ijump = "jump";
const string Ildi = "ldi";
const string Ileq = "leq";
const string Ilss = "lss";
const string Imod = "mod";
const string Imul = "mul";
const string Ineq = "neq";
const string Ipop = "pop";
const string Iprint = "print";
const string Ipush = "push";
const string Iread = "read";
const string Istorei = "storei";
const string Isub = "sub";
const string Iland = "land";
const string Ilor = "lor";
const string Iops = "ops";
const string Inot = "not";
const string Iclear = "clear";
const string Ioff = "off";
const string Ioff2 = "off2";

const int baseline = 0x82f64a83;

#endif //COMPILER_DEFINATION_H
