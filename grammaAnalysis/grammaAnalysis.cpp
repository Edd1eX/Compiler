//
// Created by 张瑞轩 on 2021/9/28.
//

#include "../include/grammaAnalysis.h"

vector<word> l;
ofstream out;
bool needPrint = true;
bool printSwitch = true;

void grammaAnalysis(vector<word> le)
{
    l = le;
    out.open("output.txt");
    CompUnitgr(0);
}

int CompUnitgr(int loc)
{
    int t = loc;
    int flag = 1;
    while(true) {
        if (get(t + 1) == MAINTK) {
            flag = 0;
            break;
        }
        if (get(t + 2) == LPARENT) {
            break;
        }
        int x = Declgr(t);
        if (x == -1) {
            break;
        }
        t = x;
    }
    while(flag) {
        if (get(t + 1) == MAINTK) {
            break;
        }
        int x = FuncDefgr(t);
        if (x == -1) {
            break;
        }
        t = x;
    }
    MainFuncDefgr(t);
    printGramma(CompUnit);
    return 0;
}
int Declgr(int loc)
{
    int t = ConstDeclgr(loc);
    if (t == -1) {
        t = VarDeclgr(loc);
        if (t == -1) {
            return -1;
        }
    }
//    printGramma(Decl);
    return t;
}
int ConstDeclgr(int loc)
{
    if (get(loc) == CONSTTK) {
        int t = nextLoc(loc);
        int x = BTypegr(t);
        if (x != -1) {
            x = ConstDefgr(x);
            if (x != -1) {
                t = x;
                while (get(t) == COMMA) {
                    t = nextLoc(t);
                    x = ConstDefgr(t);
                    t = x;
                }
                if (get(t) == SEMICN) {
                    t = nextLoc(t);
                    printGramma(ConstDecl);
                    return t;
                }
            }
        }
    }
    return -1;
}
int BTypegr(int loc)
{
    if (get(loc) == INTTK) {
        int t = nextLoc(loc);
//        printGramma(BType);
        return t;
    }
    return -1;
}
int ConstDefgr(int loc)
{
    if (get(loc) == IDENFR) {
        int t = nextLoc(loc);
        while (get(t) == LBRACK) {
            t = nextLoc(t);
            int x = ConstExpgr(t);
            // may error
            t = x;
            if (get(t) == RBRACK) {
                t = nextLoc(t);
            }
        }
        if (get(t) == ASSIGN) {
            t = nextLoc(t);
            int x = ConstInitValgr(t);
            if (x != -1) {
                printGramma(ConstDef);
                return x;
            }
        }
    }
    return -1;
}
int ConstInitValgr(int loc)
{
    int t = ConstExpgr(loc);
    if (t != -1) {
        printGramma(ConstInitVal);
        return t;
    }
    else if (get(loc) == LBRACE) {
        t = nextLoc(loc);
        int x = ConstInitValgr(t);
        if (x != -1) {
            t = x;
//            cout<<get(t)<<" "<<l[t].getWord();
            while (get(t) == COMMA) {
                t = nextLoc(t);
                x = ConstInitValgr(t);
                // error
                if (x == -1) {
                    return -1;
                }
                t = x;
            }
        }
        if (get(t) == RBRACE) {
            t = nextLoc(t);
            printGramma(ConstInitVal);
            return t;
        }
        else {
            // error
            return -1;
        }
    }
    return -1;
}
int VarDeclgr(int loc)
{
    int t = loc;
    int x;
    x = BTypegr(t);
    if (x != -1) {
        t = x;
        x = VarDefgr(t);
        if (x != -1) {
            t = x;
            while (get(t) == COMMA) {
                t = nextLoc(t);
                x = VarDefgr(t);
                t = x;
            }
            if (get(t) == SEMICN) {
                t = nextLoc(t);
                printGramma(VarDecl);
                return t;
            }
        }
    }
    return -1;
}
int VarDefgr(int loc)
{
    int t = loc;
    if (get(t) == IDENFR) {
        t = nextLoc(t);
        while (get(t) == LBRACK) {
            t = nextLoc(t);
            int x = ConstExpgr(t);
            // error
            if (x == -1) {
                return -1;
            }
            t = x;
            if (get(t) != RBRACK) {
                return -1;
            }
            t = nextLoc(t);
        }
        if (get(t) == ASSIGN) {
            t = nextLoc(t);
            int x = InitValgr(t);
//            cout<< l[t].getWord()<<endl;
            if (x != -1) {
                printGramma(VarDef);
                return x;
            }
        }
        else {
            printGramma(VarDef);
            return t;
        }
    }
    return -1;
}
int InitValgr(int loc)
{
    int t = loc;
    int x;
    x = Expgr(t);
//    cout<< l[t].getWord() << " " << x <<endl;
    if (x != -1) {
        printGramma(InitVal);
        return x;
    }
    else {
        if (get(t) == LBRACE) {
            t = nextLoc(t);
            x = InitValgr(t);
            if (x == -1) {
                return -1;
            }
            t = x;
            while (get(t) == COMMA) {
                t = nextLoc(t);
                x = InitValgr(t);
                if (x == -1) {
                    return -1;
                }
                t = x;
            }
            if (get(t) == RBRACE) {
                t = nextLoc(t);
                printGramma(InitVal);
                return t;
            }
        }
    }
    return -1;
}
int FuncDefgr(int loc)
{
    int x = FuncTypegr(loc);
    if (x == -1) {
        return -1;
    }
    int t = x;
    if (get(t) == IDENFR && get(t + 1) == LPARENT) {
        t = nextLoc(t);
        t = nextLoc(t);
        x = FuncFParamsgr(t);
        if (x != -1) {
            t = x;
        }
        if (get(t) == RPARENT) {
            t = nextLoc(t);
            x = Blockgr(t);
            if (x == -1) {
                return -1;
            }
            t = x;
            printGramma(FuncDef);
            return t;
        }
    }
    return -1;
}
int MainFuncDefgr(int loc)
{
    if (get(loc) == INTTK && get(loc + 1) == MAINTK && get(loc + 2) == LPARENT && get(loc + 3) == RPARENT)
    {
        int t = loc;
        t = nextLoc(t);
        t = nextLoc(t);
        t = nextLoc(t);
        t = nextLoc(t);
        int x = Blockgr(t);
        if (x != -1) {
            printGramma(MainFuncDef);
            return x;
        }
    }
    return -1;
}
int FuncTypegr(int loc)
{
    if (get(loc) == VOIDTK || get(loc) == INTTK) {
        int t = nextLoc(loc);
        printGramma(FuncType);
        return t;
    }
    return -1;
}
int FuncFParamsgr(int loc)
{
    int x = FuncFParamgr(loc);
    if (x == -1) {
        return -1;
    }
    int t = x;
    while (get(t) == COMMA) {
        t = nextLoc(t);
        x = FuncFParamgr(t);
        if (x == -1) {
            return -1;
        }
        t = x;
    }
    printGramma(FuncFParams);
    return t;
}
int FuncFParamgr(int loc)
{
    int x = BTypegr(loc);
    if (x == -1) {
        return -1;
    }
    int t = x;
    if (get(t) == IDENFR) {
        t = nextLoc(t);
        if (get(t) == LBRACK && get(t + 1) == RBRACK) {
            t = nextLoc(t);
            t = nextLoc(t);
            while (get(t) == LBRACK) {
                t = nextLoc(t);
                x = ConstExpgr(t);
                if (x == -1) {
                    return -1;
                }
                t = x;
                if (get(t) != RBRACK) {
                    return -1;
                }
                t = nextLoc(t);
            }
        }
        printGramma(FuncFParam);
        return t;
    }
    return -1;
}
int Blockgr(int loc)
{
    if (get(loc) == LBRACE) {
        int t = nextLoc(loc);
        while (true) {
            int x = BlockItemgr(t);
            if (x == -1) {
                break;
            }
            t = x;
        }
        if (get(t) == RBRACE) {
            t = nextLoc(t);
            printGramma(Block);
            return t;
        }
    }
    return -1;
}
int BlockItemgr(int loc)
{
    int x = Declgr(loc);
    if (x != -1) {
        return x;
    }
    x = Stmtgr(loc);
    if (x != -1) {
        return x;
    }
    return -1;
}
int Stmtgr(int loc)
{
    int t = loc;
    // Block
    int x = Blockgr(loc);
    if (x != -1) {
        printGramma(Stmt);
        return x;
    }
    // LVal = ...
    needPrint = false;
    x = LValgr(loc);
    needPrint = printSwitch;
    if (x != -1 && get(x) == ASSIGN) {
        x = LValgr(loc);
        t = nextLoc(x);
        if (get(t) == GETINTTK && get(t + 1) == LPARENT && get(t + 2) == RPARENT && get(t + 3) == SEMICN) {
            t = nextLoc(t);
            t = nextLoc(t);
            t = nextLoc(t);
            t = nextLoc(t);
            printGramma(Stmt);
            return t;
        }
        x = Expgr(t);
        if (x != -1) {
            if (get(x) == SEMICN) {
                t = nextLoc(x);
                printGramma(Stmt);
                return t;
            }
        }
    }
    // return ;
    if (get(loc) == RETURNTK) {
        t = nextLoc(t);
        x = Expgr(t);
        if (x != -1) {
            t = x;
        }
        if (get(t) == SEMICN) {
            t = nextLoc(t);
            printGramma(Stmt);
            return t;
        }
    }
    // break
    if (get(loc) == BREAKTK) {
        t = nextLoc(t);
        if (get(t) == SEMICN) {
            t = nextLoc(t);
            printGramma(Stmt);
            return t;
        }
    }
    // continue
    if (get(loc) == CONTINUETK) {
        t = nextLoc(t);
        if (get(t) == SEMICN) {
            t = nextLoc(t);
            printGramma(Stmt);
            return t;
        }
    }
    // printf
    if (get(loc) == PRINTFTK) {
        t = nextLoc(t);
        if (get(t) == LPARENT) {
            t = nextLoc(t);
            if (get(t) == STRCON) {
                t = nextLoc(t);

                while (get(t) == COMMA) {
                    t = nextLoc(t);
                    x = Expgr(t);
                    if (x == -1) {
                        return -1;
                    }
                    t = x;
                }
                if (get(t) == RPARENT && get(t + 1) == SEMICN) {
                    t = nextLoc(t);
                    t = nextLoc(t);
                    printGramma(Stmt);
                    return t;
                }
            }
        }
    }
    // if
    if (get(loc) == IFTK && get(t + 1) == LPARENT) {
        t = nextLoc(t);
        t = nextLoc(t);
        x = Condgr(t);
        if (x != -1) {
            t = x;
            if (get(t) == RPARENT) {
                t = nextLoc(t);
                x = Stmtgr(t);
                if (x != -1) {
                    t = x;
                    if (get(t) == ELSETK) {
                        t = nextLoc(t);
                        x = Stmtgr(t);
                        if (x == -1) {
                            return -1;
                        }
                        t = x;
                    }
                    printGramma(Stmt);
                    return t;
                }
            }
        }
    }
    // while
    if (get(loc) == WHILETK && get(t + 1) == LPARENT) {
        t = nextLoc(t);
        t = nextLoc(t);
        x = Condgr(t);
        if (x != -1) {
            t = x;
            if (get(t) == RPARENT) {
                t = nextLoc(t);
                x = Stmtgr(t);
                if (x != -1) {
                    printGramma(Stmt);
                    return x;
                }
            }
        }
    }
    // exp
    x = Expgr(loc);
    if (x != -1) {
        t = x;
        if (get(t) == SEMICN) {
            t = nextLoc(t);
            printGramma(Stmt);
            return t;
        }
    }
    if (get(loc) == SEMICN) {
        t = nextLoc(t);
        printGramma(Stmt);
        return t;
    }
    return -1;
}
int Expgr(int loc)
{
    int t = AddExpgr(loc);
    if (t == -1) {
        return -1;
    }
    printGramma(Exp);
    return t;
}
int Condgr(int loc)
{
    int t = LOrExpgr(loc);
    printGramma(Cond);
    return t;
}
int LValgr(int loc)
{
    if (get(loc) == IDENFR) {
        int t = nextLoc(loc);
        while (get(t) == LBRACK) {
            t = nextLoc(t);
            int x = Expgr(t);
            if (x == -1) {
                return -1;
            }
            t = x;
            if (get(t) == RBRACK) {
                t = nextLoc(t);
            }
            else {
                return -1;
            }
        }
        printGramma(LVal);
        return t;
    }
    return -1;
}
int PrimaryExpgr(int loc)
{
    int x = Numbergr(loc);
    if (x != -1) {
        printGramma(PrimaryExp);
        return x;
    }
    x = LValgr(loc);
    if (x != -1) {
        printGramma(PrimaryExp);
        return x;
    }
    if (get(loc) == LPARENT) {
        int t = nextLoc(loc);
        x = Expgr(t);
        if (x != -1) {
            t = x;
            if (get(t) == RPARENT) {
                t = nextLoc(t);
                printGramma(PrimaryExp);
                return t;
            }
        }
    }
    return -1;
}
int Numbergr(int loc)
{
    if (get(loc) == INTCON) {
        int t = nextLoc(loc);
        printGramma(Number);
        return t;
    }
    return -1;
}
int UnaryExpgr(int loc)
{
    int t = UnaryOpgr(loc);
    if (t != -1) {
        int x = UnaryExpgr(t);
        if (x != -1) {
            t = x;
            printGramma(UnaryExp);
            return t;
        }
    }
    else if (get(loc) == IDENFR && get(loc + 1) == LPARENT) {
        t = nextLoc(loc);
        if (get(t) == LPARENT) {
            t = nextLoc(t);
            int x = FuncRParamsgr(t);
            if (x != -1) {
                t = x;
            }
            if (get(t) == RPARENT) {
                t = nextLoc(t);
                printGramma(UnaryExp);
                return t;
            }
        }
    }
    else {
        int x = PrimaryExpgr(loc);
        if (x != -1) {
            printGramma(UnaryExp);
            return x;
        }
    }
    return -1;
}
int UnaryOpgr(int loc)
{
    string s = get(loc);
    if (s == PLUS || s == MINU || s == NOT) {
        int t = nextLoc(loc);
        printGramma(UnaryOp);
        return t;
    }
    return -1;
}
int FuncRParamsgr(int loc)
{
    int x = Expgr(loc);
    if (x == -1) {
        return -1;
    }
    int t = x;
    while (get(t) == COMMA) {
        t = nextLoc(t);
        x = Expgr(t);
        if (x == -1) {
            return -1;
        }
        t = x;
    }
    printGramma(FuncRParams);
    return t;
}
int MulExpgr(int loc)
{
    int t = UnaryExpgr(loc);
    if (t == -1) {
        return -1;
    }
    while (get(t) == MULT || get(t) == DIV || get(t) == MOD) {
        printGramma(MulExp);
        t = nextLoc(t);
        t = UnaryExpgr(t);
        if (t == -1) {
            return -1;
        }
    }
    printGramma(MulExp);
    return t;
}
int AddExpgr(int loc)
{
    int t = MulExpgr(loc);
    if (t == -1) {
        return -1;
    }
    while (get(t) == PLUS || get(t) == MINU) {
        printGramma(AddExp);
        t = nextLoc(t);
        t = MulExpgr(t);
        if (t == -1) {
            return -1;
        }
    }
    printGramma(AddExp);
    return t;
//    int t = nextLoc(loc);
//    printGramma(AddExp);
//    return t;
}
int RelExpgr(int loc)
{
    int t = AddExpgr(loc);
    if (t == -1) {
        return -1;
    }
    while (get(t) == LSS || get(t) == GRE || get(t) == LEQ || get(t) == GEQ) {
        printGramma(RelExp);
        t = nextLoc(t);
        t = AddExpgr(t);
        if (t == -1) {
            return -1;
        }
    }
    printGramma(RelExp);
    return t;
}
int EqExpgr(int loc)
{
    int t = RelExpgr(loc);
    if (t == -1) {
        return -1;
    }
    while (get(t) == EQL || get(t) == NEQ) {
        printGramma(EqExp);
        t = nextLoc(t);
        t = RelExpgr(t);
        if (t == -1) {
            return -1;
        }
    }
    printGramma(EqExp);
    return t;
}
int LAndExpgr(int loc)
{
    int t = EqExpgr(loc);
    if (t == -1) {
        return -1;
    }
    while (get(t) == AND) {
        printGramma(LAndExp);
        t = nextLoc(t);
        t = EqExpgr(t);
        if (t == -1) {
            return -1;
        }
    }
    printGramma(LAndExp);
    return t;
}
int LOrExpgr(int loc)
{
    int t = LAndExpgr(loc);
    if (t == -1) {
        return -1;
    }
    while (get(t) == OR) {
        printGramma(LOrExp);
        t = nextLoc(t);
        t = LAndExpgr(t);
        if (t == -1) {
            return -1;
        }
    }
    printGramma(LOrExp);
    return t;
}
int ConstExpgr(int loc)
{
    int t = AddExpgr(loc);
    if (t == -1) {
        return -1;
    }
    printGramma(ConstExp);
    return t;
}

string get(int loc)
{
    return l[loc].getCategory();
}
void error()
{
    if (needPrint) {
        cout << "error";
    }
}
int nextLoc(int loc)
{
    printLexical(loc);
    return loc + 1;
}
void printLexical(int loc)
{
    if (needPrint) {
        out << l[loc].getCategory() + " " + l[loc].getWord() << endl;
    }
}
void printGramma(string s)
{
    if (needPrint) {
        out << s << endl;
    }
}