//
// Created by 张瑞轩 on 2021/11/10.
//

#include "../include/codeGeneration.h"

extern syntax root;
vector<instr> vec;
stack<int> whilestack;
stack<int> ifstack;
stack<int> blockstack;
int while_count = 0, if_count = 0,
    assign_count = 0, and_or_count = 0;
void generation()
{
    CompUnitge(root);
    run(vec);
}

void CompUnitge(syntax node)
{
    int l = 0;
    for(;l<node.child.size();l++) {
        syntax s = node.child[l];
        if (s.syn_type != Decl) {
            break;
        }
        Declge(s);
    }
    createInstr(Ijal, "main");
    createInstr(Iend);
    for(;l<node.child.size();l++) {
        syntax s = node.child[l];
        if (s.syn_type != FuncDef) {
            break;
        }
        FuncDefge(s);
    }
    MainFuncDefge(node.child[l]);
}
void Declge(syntax node)
{
    syntax s = node.child[0];
    if (s.syn_type == ConstDecl) {
        ConstDeclge(s);
    }
    else {
        VarDeclge(s);
    }
}
void ConstDeclge(syntax node)
{
    for (syntax s:node.child) {
        if (s.syn_type == ConstDef) {
            ConstDefge(s);
        }
    }
}
void ConstDefge(syntax node)
{
    string name = node.child[0].s;
    int div = 0;
    int i = 0;
    for (syntax s:node.child) {
        if (s.category == LBRACK) {
            ConstExpge(node.child[i + 1]);
            div++;
        }
        if (s.category == ASSIGN) {
            createInstr(Ipush,div);
            createInstr(Iinit,name);
        }
        else if (s.syn_type == ConstInitVal) {
            assign_count = 0;
            ConstInitValge(s, name);
        }
        i++;
    }

}
void ConstInitValge(syntax node, string name)
{
    for(syntax s:node.child) {
        if (s.syn_type == ConstExp) {
            ConstExpge(s);
            createInstr(Ipush,assign_count);
            createInstr(Istorei,name);
            assign_count++;
        }
        else if (s.syn_type == ConstInitVal) {
            ConstInitValge(s, name);
        }
    }
}
void VarDeclge(syntax node)
{
    for (syntax s:node.child) {
        if (s.syn_type == VarDef) {
            VarDefge(s);
        }
    }
}
void VarDefge(syntax node)
{
    string name = node.child[0].s;

    int div = 0;
    for (int i = 0;i < node.child.size(); i++) {
        if (node.child[i].category == LBRACK) {
            ConstExpge(node.child[i + 1]);
            div++;
        }
    }
    createInstr(Ipush, div);
    createInstr(Iinit, name);
    for (syntax s:node.child) {
        if (s.syn_type == InitVal) {
            assign_count = 0;
            InitValge(s, name);
        }
    }
}
void InitValge(syntax node, string name)
{
    for(syntax s:node.child) {
        if (s.syn_type == Exp) {
            Expge(s);
            createInstr(Ipush, assign_count);
            createInstr(Istorei,name);
            assign_count++;
        }
        else if (s.syn_type == InitVal) {
            InitValge(s, name);
        }
    }
}
void FuncDefge(syntax node)
{
    for (syntax s : node.child) {
        if (s.category == IDENFR) {
            createInstr(1, s.s);
            createInstr(Icnb);
        }
        else if (s.syn_type == FuncFParams) {
            FuncFParamsge(s);
        }
        else if (s.syn_type == Block) {
            Blockge(s);
        }
    }
    createInstr(Idnb);
    createInstr(Ijback);
}
void MainFuncDefge(syntax node)
{
    createInstr(1,"main");
    createInstr(Icnb);
    for(syntax s:node.child) {
        if (s.syn_type == Block) {
            Blockge(s);
        }
    }
}
void FuncFParamsge(syntax node)
{
    for (syntax s : node.child) {
        if (s.syn_type == FuncFParam) {
            FuncFParamge(s);
        }
    }
}
void FuncFParamge(syntax node)
{
    string name = node.child[1].s;
    int div = 0;
    if (node.child.size() > 2) {
        div = 1;
        for (syntax s : node.child) {
            if (s.syn_type == ConstExp) {
                ConstExpge(s);
                div++;
            }
        }
        createInstr(Ipush,div);
        createInstr(Iinit2,name);
    }
    else {
        createInstr(Ipush, div);
        createInstr(Iinit, name);
        createInstr(Ipush, 0);
        createInstr(Istorei, name);
    }
}
void Blockge(syntax node)
{
    for (syntax s:node.child) {
        if (s.syn_type == BlockItem) {
            BlockItemge(s);
        }
    }
}
void BlockItemge(syntax node)
{
    if (node.child[0].syn_type == Decl) {
        Declge(node.child[0]);
    }
    else if (node.child[0].syn_type == Stmt) {
        Stmtge(node.child[0]);
    }
}
void Stmtge(syntax node)
{
    if(node.child[0].syn_type == Block) {
        if (!blockstack.empty()) {
            int x = blockstack.top() + 1;
            blockstack.pop();
            blockstack.push(x);
        }
        createInstr(Icb);
        Blockge(node.child[0]);
        createInstr(Idb);
        if (!blockstack.empty()) {
            int x = blockstack.top() - 1;
            blockstack.pop();
            blockstack.push(x);
        }
    }
    else if(node.child[0].category == RETURNTK) {
        if (node.child[1].syn_type == Exp) {
            Expge(node.child[1]);
            createInstr(Iclear, 1);
        }
        else {
            createInstr(Iclear, 0);
        }
        createInstr(Idnb);
        createInstr(Ijback);
    }
    else if(node.child[0].category == PRINTFTK) {
        for(int i = node.child.size() - 1; i > 0; i--) {
            if (node.child[i].syn_type == Exp) {
                Expge(node.child[i]);
            }
            else if (node.child[i].category == STRCON) {
                string s = node.child[i].s;
                string t = "";
                for (int j=1;j<s.size()-1;j++) {
                    if (s[j] != '%' && s[j] != '\\') {
                        t.push_back(s[j]);
                    }
                    else {
                        if (!t.empty()) {
                            createInstr(Iprint, t);
                        }
                        t="";
                        if (s[j] == '%' && s[j+1] == 'd') {
                            j++;
                            createInstr(Iprint);
                        }
                        else if (s[j] == '\\' && s[j+1] == 'n') {
                            j++;
                            createInstr(Iprint, "\n");
                        }
                    }
                }
                if (!t.empty()) {
                    createInstr(Iprint, t);
                }
            }
        }
    }
    else if(node.child[0].syn_type == LVal) {
        if (node.child[2].syn_type == Exp) {
            Expge(node.child[2]);
        }
        else if (node.child[2].category == GETINTTK) {
            createInstr(Iread);
        }
        LValAssign(node.child[0]);
    }
    else if(node.child[0].syn_type == Exp) {
        Expge(node.child[0]);
    }
    else if(node.child[0].category == IFTK) {
        ifstack.push(if_count);
        if_count++;
        Condge(node.child[2]);
        string s = make_if_label(1);
        createInstr(Ibez,s);
        Stmtge(node.child[4]);
        createInstr(Ijump, make_if_label(2));
        createInstr(1,s);
        if (node.child.size() > 5) {
            Stmtge(node.child[6]);
        }
        s = make_if_label(2);
        createInstr(1,s);
        ifstack.pop();
    }
    else if(node.child[0].category == WHILETK) {
        whilestack.push(while_count);
        blockstack.push(0);

        while_count++;
        string s = make_while_label(1);
        createInstr(1,s);
        Condge(node.child[2]);
        createInstr(Ibez,make_while_label(2));
        Stmtge(node.child[4]);
        createInstr(Ijump, make_while_label(1));
        createInstr(1, make_while_label(2));

        whilestack.pop();
        blockstack.pop();
    }
    else if(node.child[0].category == BREAKTK) {
        int x = blockstack.top();
        while (x--) {
            createInstr(Idb);
        }
        createInstr(Ijump, make_while_label(2));
    }
    else if(node.child[0].category == CONTINUETK) {
        int x = blockstack.top();
        while (x--) {
            createInstr(Idb);
        }
        createInstr(Ijump, make_while_label(1));
    }
}
void Expge(syntax node)
{
    if (node.child[0].syn_type == AddExp) {
        AddExpge(node.child[0]);
    }
}
void Condge(syntax node)
{
    if (node.child[0].syn_type == LOrExp) {
        LOrExpge(node.child[0]);
    }
}
void LValge(syntax node)
{
    string name = node.child[0].s;
    int div = 0;
    for (syntax s : node.child) {
        if (s.syn_type == Exp) {
            div++;
            Expge(s);
        }
    }
    createInstr(Ioff2, div, name);
//    createInstr(Ildi, name);
}
void LValAssign(syntax node)
{
    string name = node.child[0].s;
    int div = 0;
    for (syntax s : node.child) {
        if (s.syn_type == Exp) {
            div++;
            Expge(s);
        }
    }
    createInstr(Ioff, div, name);
    createInstr(Istorei, name);
}
void PrimaryExpge(syntax node)
{
    if (node.child[0].syn_type == Number) {
        Numberge(node.child[0]);
    }
    else if (node.child[0].syn_type == LVal) {
        LValge(node.child[0]);
    }
    else {
        if (node.child[1].syn_type == Exp) {
            Expge(node.child[1]);
        }
    }
}
void Numberge(syntax node)
{
    int x = atoi(node.child[0].s.c_str());
    createInstr(Ipush, x);
}
void UnaryExpge(syntax node)
{
    if (node.child[0].syn_type == UnaryOp) {
        if (node.child[1].syn_type == UnaryExp) {
            UnaryExpge(node.child[1]);
        }
        UnaryOpge(node.child[0]);
    }
    else if (node.child[0].syn_type == PrimaryExp) {
        PrimaryExpge(node.child[0]);
    }
    else {
        string name = node.child[0].s;
        createInstr(Ipush, baseline);
        if (node.child[2].syn_type == FuncRParams) {
            FuncRParamsge(node.child[2]);
        }
        createInstr(Ijal, name);
    }
}
void UnaryOpge(syntax node)
{
    if (node.child[0].category == MINU) {
        createInstr(Iops);
    }
    else if (node.child[0].category == NOT) {
        createInstr(Inot);
    }
}
void FuncRParamsge(syntax node)
{
    for (int i = node.child.size() - 1; i >= 0; i--) {
        syntax s = node.child[i];
        if (s.syn_type == Exp) {
            Expge(s);
        }
    }
}
void MulExpge(syntax node)
{
    if (node.child[0].syn_type == UnaryExp) {
        UnaryExpge(node.child[0]);
    }
    else {
        int flag = 0;
        for(syntax s : node.child) {
            if (s.syn_type == UnaryExp) {
                UnaryExpge(s);
            }
            else if (s.syn_type == MulExp) {
                MulExpge(s);
            }
            else {
                if (s.category == MULT) {
                    flag = 1;
                }
                else if (s.category == DIV) {
                    flag = 2;
                }
                else if (s.category == MOD) {
                    flag = 3;
                }
            }
        }
        switch (flag) {
            case 1:
                createInstr(Imul);
                break;
            case 2:
                createInstr(Idiv);
                break;
            case 3:
                createInstr(Imod);
                break;
            default:
                cout<<"error on MulExp..."<<endl;
        }
    }
}
void AddExpge(syntax node)
{
    if (node.child[0].syn_type == MulExp) {
        MulExpge(node.child[0]);
    }
    else {
        int flag = 0;
        for(syntax s : node.child) {
            if (s.syn_type == AddExp) {
                AddExpge(s);
            }
            else if (s.syn_type == MulExp) {
                MulExpge(s);
            }
            else {
                if (s.category == PLUS) {
                    flag = 1;
                }
                else if (s.category == MINU) {
                    flag = 2;
                }
            }
        }
        switch (flag) {
            case 1:
                createInstr(Iadd);
                break;
            case 2:
                createInstr(Isub);
                break;
            default:
                cout<<"error on AddExp..."<<endl;
        }
    }
}
void RelExpge(syntax node)
{
    if (node.child[0].syn_type == AddExp) {
        AddExpge(node.child[0]);
    }
    else {
        int flag = 0;
        for(syntax s : node.child) {
            if (s.syn_type == RelExp) {
                RelExpge(s);
            }
            else if (s.syn_type == AddExp) {
                AddExpge(s);
            }
            else {
                if (s.category == LSS) {
                    flag = 1;
                }
                else if (s.category == GRE) {
                    flag = 2;
                }
                else if (s.category == LEQ) {
                    flag = 3;
                }
                else if (s.category == GEQ) {
                    flag = 4;
                }
            }
        }
        switch (flag) {
            case 1:
                createInstr(Ilss);
                break;
            case 2:
                createInstr(Igt);
                break;
            case 3:
                createInstr(Ileq);
                break;
            case 4:
                createInstr(Igte);
                break;
            default:
                cout<<"error on RelExp..."<<endl;
        }
    }
}
void EqExpge(syntax node)
{
    if (node.child[0].syn_type == RelExp) {
        RelExpge(node.child[0]);
    }
    else {
        int flag = 0;
        for(syntax s : node.child) {
            if (s.syn_type == EqExp) {
                EqExpge(s);
            }
            else if (s.syn_type == RelExp) {
                RelExpge(s);
            }
            else {
                if (s.category == EQL) {
                    flag = 1;
                }
                else if (s.category == NEQ) {
                    flag = 2;
                }
            }
        }
        switch (flag) {
            case 1:
                createInstr(Ieq);
                break;
            case 2:
                createInstr(Ineq);
                break;
            default:
                cout<<"error on EqExp..."<<endl;
        }
    }
}
void LAndExpge(syntax node)
{
    if (node.child[0].syn_type == EqExp) {
        EqExpge(node.child[0]);
    }
    else {
        int flag = 0;
        int x = and_or_count;
        and_or_count++;
        for(syntax s : node.child) {
            if (s.syn_type == LAndExp) {
                LAndExpge(s);
            }
            else if (s.syn_type == EqExp) {
                EqExpge(s);
            }
            else {
                if (s.category == AND) {
                    createInstr(Icopy);
                    createInstr(Ibez, make_and_or_label(x));
                    flag = 1;
                }
            }
        }
        switch (flag) {
            case 1:
                createInstr(Iland);
                createInstr(1, make_and_or_label(x));
                break;
            default:
                cout<<"error on LAndExp..."<<endl;
        }
    }
}
void LOrExpge(syntax node)
{
    if (node.child[0].syn_type == LAndExp) {
        LAndExpge(node.child[0]);
    }
    else {
        int flag = 0;
        int x = and_or_count;
        and_or_count++;
        for(syntax s : node.child) {
            if (s.syn_type == LAndExp) {
                LAndExpge(s);
            }
            else if (s.syn_type == LOrExp) {
                LOrExpge(s);
            }
            else {
                if (s.category == OR) {
                    createInstr(Icopy);
                    createInstr(Ibnz, make_and_or_label(x));
                    flag = 1;
                }
            }
        }
        switch (flag) {
            case 1:
                createInstr(Ilor);
                createInstr(1, make_and_or_label(x));
                break;
            default:
                cout<<"error on LOrExp..."<<endl;
        }
    }
}
void ConstExpge(syntax node)
{
    if (node.child[0].syn_type == AddExp) {
        AddExpge(node.child[0]);
    }
}
void createInstr(string name, int x)
{
    vec.emplace_back(name,x);
}
void createInstr(string name, string y)
{
    vec.emplace_back(name,y);
}
void createInstr(string name, int x, string y)
{
    vec.emplace_back(name, x, y);
}
void createInstr(int flag, string name)
{
    vec.emplace_back(flag,name);
}
void createInstr(string name)
{
    vec.emplace_back(name);
}
string make_if_label(int flag)
{
    int x = ifstack.top();
    switch (flag) {
        case 1:
            return "if_false_" + to_string(x);
        case 2:
            return "if_end_" + to_string(x);
        default:
            return "";
    }
}
string make_while_label(int flag)
{
    int x = whilestack.top();
    switch (flag) {
        case 1:
            return "while_loop_" + to_string(x);
        case 2:
            return "while_end_" + to_string(x);
        default:
            return "";
    }
}
string make_and_or_label(int x)
{
     return "and_or_" + to_string(x);
}