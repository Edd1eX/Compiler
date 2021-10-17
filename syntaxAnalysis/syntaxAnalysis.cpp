//
// Created by 张瑞轩 on 2021/10/16.
//

//
// Created by 张瑞轩 on 2021/9/28.
//

#include "../include/syntaxAnalysis.h"

vector<word> l;
syntax root(CompUnit);
ofstream out;
int loc = 0;

void syntaxAnalysis(vector<word> le)
{
    l = le;
    root = CompUnitgr();
    out.open("output.txt");
    output(root);
}
syntax CompUnitgr()
{
    syntax node(CompUnit);
    int flag = 1;
    while(true) {
        if (get(loc + 1) == MAINTK) {
            flag = 0;
            break;
        }
        if (get(loc + 2) == LPARENT) {
            break;
        }
        syntax s = Declgr();
        if (s.iserror()) {
            break;
        }
        node.insert(s);
    }
    while(flag) {
        if (get(loc + 1) == MAINTK) {
            break;
        }
        syntax s = FuncDefgr();
        if (s.iserror()) {
            break;
        }
        node.insert(s);
    }
    syntax s = MainFuncDefgr();
    node.insert(s);
    return node;
}
syntax Declgr()
{
    syntax node(Decl);
    syntax s = ConstDeclgr();
    if (s.iserror()) {
        s = VarDeclgr();
        if (s.iserror()) {
            return error();
        }
    }
    node.insert(s);
    return node;
}
syntax ConstDeclgr()
{
    syntax node(ConstDecl);
    if (get(loc) == CONSTTK) {
        syntax s = get_next();
        node.insert(s);

        s = BTypegr();
        if (!s.iserror()) {
            node.insert(s);
            s = ConstDefgr();
            if (!s.iserror()) {
                node.insert(s);
                while (get(loc) == COMMA) {
                    s = get_next();
                    node.insert(s);
                    s = ConstDefgr();
                    node.insert(s);
                }
                if (get(loc) == SEMICN) {
                    s = get_next();
                    node.insert(s);
                    return node;
                }
            }
        }
    }
    return error();
}
syntax BTypegr()
{
    syntax node(BType);
    if (get(loc) == INTTK) {
        syntax s = get_next();
        node.insert(s);
        return node;
    }
    return error();
}
syntax ConstDefgr()
{
    syntax node(ConstDef);
    if (get(loc) == IDENFR) {
        syntax s = get_next();
        node.insert(s);
        while (get(loc) == LBRACK) {
            s = get_next();
            node.insert(s);

            s = ConstExpgr();
            node.insert(s);
            // may error
            if (get(loc) == RBRACK) {
                s = get_next();
                node.insert(s);
            }
        }
        if (get(loc) == ASSIGN) {
            s = get_next();
            node.insert(s);

            s = ConstInitValgr();
            if (!s.iserror()) {
                node.insert(s);
                return node;
            }
        }
    }
    return error();
}
syntax ConstInitValgr()
{
    syntax node(ConstInitVal);
    syntax s = ConstExpgr();
    if (!s.iserror()) {
        node.insert(s);
        return node;
    }
    else if (get(loc) == LBRACE) {
        s = get_next();
        node.insert(s);
        s = ConstInitValgr();
        if (!s.iserror()) {
            node.insert(s);
            while (get(loc) == COMMA) {
                s = get_next();
                node.insert(s);

                s = ConstInitValgr();
                // error
                if (s.iserror()) {
                    return error();
                }
                node.insert(s);
            }
        }
        if (get(loc) == RBRACE) {
            s = get_next();
            node.insert(s);
            return node;
        }
        else {
            // error
            return error();
        }
    }
    return error();
}
syntax VarDeclgr()
{
    syntax node(VarDecl);
    syntax s = BTypegr();
    if (!s.iserror()) {
        node.insert(s);
        s = VarDefgr();
        if (!s.iserror()) {
            node.insert(s);
            while (get(loc) == COMMA) {
                s = get_next();
                node.insert(s);
                s = VarDefgr();
                node.insert(s);
            }
            if (get(loc) == SEMICN) {
                s = get_next();
                node.insert(s);
                return node;
            }
        }
    }
    return error();
}
syntax VarDefgr()
{
    syntax node(VarDef);
    if (get(loc) == IDENFR) {
        syntax s = get_next();
        node.insert(s);
        while (get(loc) == LBRACK) {
            s = get_next();
            node.insert(s);

            s = ConstExpgr();
            // error
            if (s.iserror()) {
                return error();
            }
            node.insert(s);
            if (get(loc) != RBRACK) {
                return error();
            }
            s = get_next();
            node.insert(s);
        }
        if (get(loc) == ASSIGN) {
            s = get_next();
            node.insert(s);
            s = InitValgr();
            if (!s.iserror()) {
                node.insert(s);
                return node;
            }
        }
        else {
            return node;
        }
    }
    return error();
}
syntax InitValgr()
{
    syntax node(InitVal);
    syntax s = Expgr();
    if (!s.iserror()) {
        node.insert(s);
        return node;
    }
    else {
        if (get(loc) == LBRACE) {
            s = get_next();
            node.insert(s);
            s = InitValgr();
            if (s.iserror()) {
                return error();
            }
            node.insert(s);
            while (get(loc) == COMMA) {
                s = get_next();
                node.insert(s);

                s = InitValgr();
                if (s.iserror()) {
                    return error();
                }
                node.insert(s);
            }
            if (get(loc) == RBRACE) {
                s = get_next();
                node.insert(s);
                return node;
            }
        }
    }
    return error();
}
syntax FuncDefgr()
{
    syntax node(FuncDef);
    syntax s = FuncTypegr();
    if (s.iserror()) {
        return error();
    }
    node.insert(s);
    if (get(loc) == IDENFR && get(loc + 1) == LPARENT) {
        s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);

        s = FuncFParamsgr();
        if (!s.iserror()) {
            node.insert(s);
        }
        if (get(loc) == RPARENT) {
            s = get_next();
            node.insert(s);
            s = Blockgr();
            if (s.iserror()) {
                return error();
            }
            node.insert(s);
            return node;
        }
    }
    return error();
}
syntax MainFuncDefgr()
{
    syntax node(MainFuncDef);
    if (get(loc) == INTTK && get(loc + 1) == MAINTK && get(loc + 2) == LPARENT && get(loc + 3) == RPARENT)
    {
        syntax s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);

        s = Blockgr();
        if (!s.iserror()) {
            node.insert(s);
            return node;
        }
    }
    return error();
}
syntax FuncTypegr()
{
    syntax node(FuncType);
    if (get(loc) == VOIDTK || get(loc) == INTTK) {
        syntax s = get_next();
        node.insert(s);
        return node;
    }
    return error();
}
syntax FuncFParamsgr()
{
    syntax node(FuncFParams);
    syntax s = FuncFParamgr();
    if (s.iserror()) {
        return error();
    }
    node.insert(s);
    while (get(loc) == COMMA) {
        s = get_next();
        node.insert(s);

        s = FuncFParamgr();
        if (s.iserror()) {
            return error();
        }
        node.insert(s);
    }
    return node;
}
syntax FuncFParamgr()
{
    syntax node(FuncFParam);
    syntax s = BTypegr();
    if (s.iserror()) {
        return error();
    }
    node.insert(s);
    if (get(loc) == IDENFR) {
        s = get_next();
        node.insert(s);
        if (get(loc) == LBRACK && get(loc + 1) == RBRACK) {
            s = get_next();
            node.insert(s);
            s = get_next();
            node.insert(s);
            while (get(loc) == LBRACK) {
                s = get_next();
                node.insert(s);
                s = ConstExpgr();
                if (s.iserror()) {
                    return error();
                }
                node.insert(s);
                if (get(loc) != RBRACK) {
                    return error();
                }
                s = get_next();
                node.insert(s);
            }
        }
        return node;
    }
    return error();
}
syntax Blockgr()
{
    syntax node(Block);
    if (get(loc) == LBRACE) {
        syntax s = get_next();
        node.insert(s);
        while (true) {
            s = BlockItemgr();
            if (s.iserror()) {
                break;
            }
            node.insert(s);
        }
        if (get(loc) == RBRACE) {
            s = get_next();
            node.insert(s);
            return node;
        }
    }
    return error();
}
syntax BlockItemgr()
{
    syntax node(BlockItem);
    syntax s = Declgr();
    if (!s.iserror()) {
        node.insert(s);
        return node;
    }
    s = Stmtgr();
    if (!s.iserror()) {
        node.insert(s);
        return node;
    }
    return error();
}
syntax Stmtgr()
{
    // TODO
    syntax node(Stmt);
    // Block
    syntax s = Blockgr();
    if (!s.iserror()) {
        node.insert(s);
        return node;
    }
    // LVal = ...
    int loc_temp = loc;
    s = LValgr();
    if (!s.iserror() && get(loc) == ASSIGN) {
        node.insert(s);
        s = get_next();
        node.insert(s);
        if (get(loc) == GETINTTK && get(loc + 1) == LPARENT && get(loc + 2) == RPARENT && get(loc + 3) == SEMICN) {
            s = get_next();
            node.insert(s);
            s = get_next();
            node.insert(s);
            s = get_next();
            node.insert(s);
            s = get_next();
            node.insert(s);
            return node;
        }
        s = Expgr();
        if (!s.iserror()) {
            node.insert(s);
            if (get(loc) == SEMICN) {
                s = get_next();
                node.insert(s);
                return node;
            }
        }
    }
    loc = loc_temp;
    // return ;
    if (get(loc) == RETURNTK) {
        s = get_next();
        node.insert(s);
        s = Expgr();
        if (!s.iserror()) {
            node.insert(s);
        }
        if (get(loc) == SEMICN) {
            s = get_next();
            node.insert(s);
            return node;
        }
    }
    // break
    if (get(loc) == BREAKTK) {
        s = get_next();
        node.insert(s);
        if (get(loc) == SEMICN) {
            s = get_next();
            node.insert(s);
            return node;
        }
    }
    // continue
    if (get(loc) == CONTINUETK) {
        s = get_next();
        node.insert(s);
        if (get(loc) == SEMICN) {
            s = get_next();
            node.insert(s);
            return node;
        }
    }
    // printf
    if (get(loc) == PRINTFTK) {
        s = get_next();
        node.insert(s);
        if (get(loc) == LPARENT) {
            s = get_next();
            node.insert(s);
            if (get(loc) == STRCON) {
                s = get_next();
                node.insert(s);

                while (get(loc) == COMMA) {
                    s = get_next();
                    node.insert(s);
                    s = Expgr();
                    if (s.iserror()) {
                        return error();
                    }
                    node.insert(s);
                }
                if (get(loc) == RPARENT && get(loc + 1) == SEMICN) {
                    s = get_next();
                    node.insert(s);
                    s = get_next();
                    node.insert(s);
                    return node;
                }
            }
        }
    }
    // if
    if (get(loc) == IFTK && get(loc + 1) == LPARENT) {
        s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);
        s = Condgr();
        if (!s.iserror()) {
            node.insert(s);
            if (get(loc) == RPARENT) {
                s = get_next();
                node.insert(s);
                s = Stmtgr();
                if (!s.iserror()) {
                    node.insert(s);
                    if (get(loc) == ELSETK) {
                        s = get_next();
                        node.insert(s);
                        s = Stmtgr();
                        if (s.iserror()) {
                            return error();
                        }
                        node.insert(s);
                    }
                    return node;
                }
            }
        }
    }
    // while
    if (get(loc) == WHILETK && get(loc + 1) == LPARENT) {
        s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);
        s = Condgr();
        if (!s.iserror()) {
            node.insert(s);
            if (get(loc) == RPARENT) {
                s = get_next();
                node.insert(s);
                s = Stmtgr();
                if (!s.iserror()) {
                    node.insert(s);
                    return node;
                }
            }
        }
    }
    // exp
    s = Expgr();
    if (!s.iserror()) {
        node.insert(s);
        if (get(loc) == SEMICN) {
            s = get_next();
            node.insert(s);
            return node;
        }
    }
    if (get(loc) == SEMICN) {
        s = get_next();
        node.insert(s);
        return node;
    }
    return error();
}
syntax Expgr()
{
    syntax node(Exp);
    syntax s = AddExpgr();
    if (s.iserror()) {
        return error();
    }
    node.insert(s);
    return node;
}
syntax Condgr()
{
    syntax node(Cond);
    syntax s = LOrExpgr();
    node.insert(s);
    return node;
}
syntax LValgr()
{
    syntax node(LVal);
    if (get(loc) == IDENFR) {
        syntax s = get_next();
        node.insert(s);
        while (get(loc) == LBRACK) {
            s = get_next();
            node.insert(s);

            s = Expgr();
            if (s.iserror()) {
                return error();
            }
            node.insert(s);
            if (get(loc) == RBRACK) {
                s = get_next();
                node.insert(s);
            }
            else {
                return error();
            }
        }
        return node;
    }
    return error();
}
syntax PrimaryExpgr()
{
    syntax node(PrimaryExp);
    syntax s = Numbergr();
    if (!s.iserror()) {
        node.insert(s);
        return node;
    }
    s = LValgr();
    if (!s.iserror()) {
        node.insert(s);
        return node;
    }
    if (get(loc) == LPARENT) {
        s = get_next();
        node.insert(s);
        s = Expgr();
        if (!s.iserror()) {
            node.insert(s);
            if (get(loc) == RPARENT) {
                s = get_next();
                node.insert(s);
                return node;
            }
        }
    }
    return error();
}
syntax Numbergr()
{
    syntax node(Number);
    if (get(loc) == INTCON) {
        syntax s = get_next();
        node.insert(s);
        return node;
    }
    return error();
}
syntax UnaryExpgr()
{
    syntax node(UnaryExp);
    syntax s = UnaryOpgr();
    if (!s.iserror()) {
        node.insert(s);
        s = UnaryExpgr();
        if (!s.iserror()) {
            node.insert(s);
            return node;
        }
    }
    else if (get(loc) == IDENFR && get(loc + 1) == LPARENT) {
        s = get_next();
        node.insert(s);
        if (get(loc) == LPARENT) {
            s = get_next();
            node.insert(s);
            s = FuncRParamsgr();
            if (!s.iserror()) {
                node.insert(s);
            }
            if (get(loc) == RPARENT) {
                s = get_next();
                node.insert(s);
                return node;
            }
        }
    }
    else {
        s = PrimaryExpgr();
        if (!s.iserror()) {
            node.insert(s);
            return node;
        }
    }
    return error();
}
syntax UnaryOpgr()
{
    syntax node(UnaryOp);
    if (get(loc) == PLUS || get(loc) == MINU || get(loc) == NOT) {
        syntax s = get_next();
        node.insert(s);
        return node;
    }
    return error();
}
syntax FuncRParamsgr()
{
    syntax node(FuncRParams);
    syntax s = Expgr();
    if (s.iserror()) {
        return error();
    }
    node.insert(s);
    while (get(loc) == COMMA) {
        s = get_next();
        node.insert(s);
        s = Expgr();
        if (s.iserror()) {
            return error();
        }
        node.insert(s);
    }
    return node;
}
syntax MulExpgr()
{
    syntax s = UnaryExpgr();
    if (s.iserror()) {
        return error();
    }
    vector<syntax> child;

    while (get(loc) == MULT || get(loc) == DIV || get(loc) == MOD) {
        syntax a(MulExp);

        syntax ss = get_next();
        a.insert(ss);
        ss = UnaryExpgr();
        if (ss.iserror()) {
            return error();
        }
        a.insert(ss);
        child.push_back(a);
    }
    syntax node(MulExp);
    node.insert(s);
    reverse(child.begin(), child.end());
    child.push_back(node);
    for (int i = child.size() - 2; i >= 0;i--) {
        syntax *p = &child[i];
        auto it = (p->child).begin();
        (p->child).insert(it, child[i + 1]);
    }
    return child.front();
}
syntax AddExpgr()
{
    syntax s = MulExpgr();
    if (s.iserror()) {
        return error();
    }
    vector<syntax> child;

    while (get(loc) == PLUS || get(loc) == MINU) {
        syntax a(AddExp);

        syntax ss = get_next();
        a.insert(ss);
        ss = MulExpgr();
        if (ss.iserror()) {
            return error();
        }
        a.insert(ss);
        child.push_back(a);
    }
    syntax node(AddExp);
    node.insert(s);
    reverse(child.begin(), child.end());
    child.push_back(node);
    for (int i = child.size() - 2; i >= 0;i--) {
        syntax *p = &child[i];
        auto it = (p->child).begin();
        (p->child).insert(it, child[i + 1]);
    }
    return child.front();
}
syntax RelExpgr()
{
    syntax s = AddExpgr();
    if (s.iserror()) {
        return error();
    }
    vector<syntax> child;

    while (get(loc) == LSS || get(loc) == GRE || get(loc) == LEQ || get(loc) == GEQ) {
        syntax a(RelExp);

        syntax ss = get_next();
        a.insert(ss);
        ss = AddExpgr();
        if (ss.iserror()) {
            return error();
        }
        a.insert(ss);
        child.push_back(a);
    }
    syntax node(RelExp);
    node.insert(s);
    reverse(child.begin(), child.end());
    child.push_back(node);
    for (int i = child.size() - 2; i >= 0;i--) {
        syntax *p = &child[i];
        auto it = (p->child).begin();
        (p->child).insert(it, child[i + 1]);
    }
    return child.front();
}
syntax EqExpgr()
{
    syntax s = RelExpgr();
    if (s.iserror()) {
        return error();
    }
    vector<syntax> child;

    while (get(loc) == EQL || get(loc) == NEQ) {
        syntax a(EqExp);

        syntax ss = get_next();
        a.insert(ss);
        ss = RelExpgr();
        if (ss.iserror()) {
            return error();
        }
        a.insert(ss);
        child.push_back(a);
    }
    syntax node(EqExp);
    node.insert(s);
    reverse(child.begin(), child.end());
    child.push_back(node);
    for (int i = child.size() - 2; i >= 0;i--) {
        syntax *p = &child[i];
        auto it = (p->child).begin();
        (p->child).insert(it, child[i + 1]);
    }
    return child.front();
}
syntax LAndExpgr()
{
    syntax s = EqExpgr();
    if (s.iserror()) {
        return error();
    }
    vector<syntax> child;

    while (get(loc) == AND) {
        syntax a(LAndExp);

        syntax ss = get_next();
        a.insert(ss);
        ss = EqExpgr();
        if (ss.iserror()) {
            return error();
        }
        a.insert(ss);
        child.push_back(a);
    }
    syntax node(LAndExp);
    node.insert(s);
    reverse(child.begin(), child.end());
    child.push_back(node);
    for (int i = child.size() - 2; i >= 0;i--) {
        syntax *p = &child[i];
        auto it = (p->child).begin();
        (p->child).insert(it, child[i + 1]);
    }
    return child.front();
}
syntax LOrExpgr()
{
    syntax s = LAndExpgr();
    if (s.iserror()) {
        return error();
    }
    vector<syntax> child;

    while (get(loc) == OR) {
        syntax a(LOrExp);

        syntax ss = get_next();
        a.insert(ss);
        ss = LAndExpgr();
        if (ss.iserror()) {
            return error();
        }
        a.insert(ss);
        child.push_back(a);
    }
    syntax node(LOrExp);
    node.insert(s);
    reverse(child.begin(), child.end());
    child.push_back(node);
    for (int i = child.size() - 2; i >= 0;i--) {
        syntax *p = &child[i];
        auto it = (p->child).begin();
        (p->child).insert(it, child[i + 1]);
    }
    return child.front();
}
syntax ConstExpgr()
{
    syntax node(ConstExp);
    syntax s = AddExpgr();
    if (s.iserror()) {
        return error();
    }
    node.insert(s);
    return node;
}

syntax error()
{
    return syntax(-1);
}
string get(int t)
{
    return l[t].getCategory();
}
syntax get_next()
{
    syntax s(l[loc]);
    loc++;
    return s;
}
void output(syntax s)
{
    for (syntax t : s.child) {
        output(t);
    }
    if (s.flag == 0) {
        out << s.category + " " + s.s << endl;
    }
    else if (s.flag == 1) {
        if (s.syn_type != BlockItem &&
            s.syn_type != Decl &&
            s.syn_type != BType)
            out << s.syn_type << endl;
    }
}