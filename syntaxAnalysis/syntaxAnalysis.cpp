//
// Created by 张瑞轩 on 2021/9/28.
//

#include "../include/syntaxAnalysis.h"

vector<word> l;
vector<errorType> errors;
syntax root(CompUnit);
ofstream out;
symbolStack symbols;

int loc = 0;
int loop_layer = 0;

void syntaxAnalysis(vector<word> le)
{
    l = le;
    root = CompUnitgr();
    // out.open("output.txt");
    // output(root);
    errorAnalysis();
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
                else {
                    // i
                    push_error("i", loc - 1);
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
        string name = l[loc].getWord();
        syntax s = get_next();
        node.insert(s);
        int dim = 0;
        while (get(loc) == LBRACK) {
            s = get_next();
            node.insert(s);

            s = ConstExpgr();
            node.insert(s);
            dim++;
            // may error
            if (get(loc) == RBRACK) {
                s = get_next();
                node.insert(s);
            }
            // k
            else {
                push_error("k", loc - 1);
            }
        }
        // b
        if(symbols.top()->find_on_def(name)) {
            push_error("b", loc - 1);
        }
        else {
            symbols.top()->push(symbol(dim,name, "const", "int"));
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
            else {
                // i
                push_error("i", loc - 1);
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
        string name = l[loc].getWord();
        syntax s = get_next();
        node.insert(s);
        int dim = 0;
        while (get(loc) == LBRACK) {
            s = get_next();
            node.insert(s);

            dim++;
            s = ConstExpgr();
            // error
            if (s.iserror()) {
                return error();
            }
            node.insert(s);
            if (get(loc) == RBRACK) {
                s = get_next();
                node.insert(s);
            }
            // k
            else {
                push_error("k", loc - 1);
            }
        }

        // b
        if(symbols.top()->find_on_def(name)) {
            push_error("b", loc - 1);
        }
        else {
            symbols.top()->push(symbol(dim, name, "var", "int"));
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

    string type = s.getType();

    node.insert(s);
    if (get(loc) == IDENFR && get(loc + 1) == LPARENT) {
        string name = l[loc].getWord();
        int flag = 0;
        // b
        if(symbols.top()->find_on_def(name)) {
            push_error("b", loc);
            flag = 1;
        }
        else {
            symbols.top()->push(symbol(name, "function", type));
        }

        s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);

        symbols.push_new(type == "void" ? 2 : 1);

        s = FuncFParamsgr();
        vector<symbol> syms = s.get_param();
        if (!s.iserror()) {
            node.insert(s);
        }
        if (get(loc) == RPARENT) {
            s = get_next();
            node.insert(s);
        }
        // j
        else {
            push_error("j", loc - 1);
        }
        if (flag == 0) {
            int params = symbols.top()->symbols.size();
            symbol *sym = symbols.top()->get(name);
            sym->params = params;
            sym->param = syms;
        }
        s = Blockgr();
        if (s.iserror()) {
            return error();
        }
        node.insert(s);
        return node;
    }
    return error();
}
syntax MainFuncDefgr()
{
    syntax node(MainFuncDef);

    if (get(loc) == INTTK && get(loc + 1) == MAINTK && get(loc + 2) == LPARENT)
    {
        syntax s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);
        s = get_next();
        node.insert(s);
        if (get(loc) == RPARENT){
            s = get_next();
            node.insert(s);
        }
        // j
        else {
            push_error("j", loc - 1);
        }
        symbols.push_new(1);
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
    string type = s.getType();
    node.insert(s);
    if (get(loc) == IDENFR) {
        string name = l[loc].getWord();
        s = get_next();
        node.insert(s);
        if (get(loc) == LBRACK) {
            s = get_next();
            node.insert(s);
            if (get(loc) == RBRACK) {
                s = get_next();
                node.insert(s);
            }
            // k
            else {
                push_error("k", loc - 1);
            }
            while (get(loc) == LBRACK) {
                s = get_next();
                node.insert(s);
                s = ConstExpgr();
                if (s.iserror()) {
                    return error();
                }
                node.insert(s);
                if (get(loc) == RBRACK) {
                    s = get_next();
                    node.insert(s);
                }
                // k
                else {
                    push_error("k", loc - 1);
                }
            }
        }
        // b
        if(symbols.top()->find_on_def(name)) {
            push_error("b", loc - 1);
        }
        else {
            symbols.top()->push(symbol(name, "var", type));
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
        // g
        if (symbols.top()->funcType == 1 && !node.checkLast()) {
            push_error("g", loc);
        }
        symbols.pop();

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
    syntax node(Stmt);
    // Block
    symbols.push_new(0);
    syntax s = Blockgr();
    if (!s.iserror()) {
        node.insert(s);
        return node;
    }
    else {
        symbols.pop();
    }
    // LVal = ...
    int loc_temp = loc;
    s = LValgr();
    if (!s.iserror() && get(loc) == ASSIGN) {
        string name = s.getName();
        if (symbols.top()->find(name)) {
            symbol *sym = symbols.top()->get(name);
            // h
            if (sym->kind == "const") {
                push_error("h", loc - 1);
            }
        }
        node.insert(s);
        s = get_next();
        node.insert(s);
        if (get(loc) == GETINTTK && get(loc + 1) == LPARENT) {
            s = get_next();
            node.insert(s);
            s = get_next();
            node.insert(s);
            if (get(loc) == RPARENT) {
                s = get_next();
                node.insert(s);
            }
            // j
            else {
                push_error("j", loc - 1);
            }
            if (get(loc) == SEMICN) {
                s = get_next();
                node.insert(s);
            }
            else {
                // i
                push_error("i", loc - 1);
            }
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
            else {
                // i
                push_error("i", loc - 1);
                return node;
            }
        }
    }
    loc = loc_temp;
    // return ;
    if (get(loc) == RETURNTK) {
        int lo = loc;
        s = get_next();
        node.insert(s);
        s = Expgr();
        if (!s.iserror()) {
            symbols.top()->is_return = true;
            // f
            if (!symbols.top()->is_right_return()) {
                push_error("f", lo);
            }
            node.insert(s);
        }
        if (get(loc) == SEMICN) {
            s = get_next();
            node.insert(s);
            return node;
        }
        else {
            // i
            push_error("i", loc - 1);
            return node;
        }
    }
    // break
    if (get(loc) == BREAKTK) {
        // m
        if (loop_layer <= 0) {
            loop_layer = 0;
            push_error("m", loc);
        }
        s = get_next();
        node.insert(s);
        if (get(loc) == SEMICN) {
            s = get_next();
            node.insert(s);
            return node;
        }
        else {
            // i
            push_error("i", loc - 1);
            return node;
        }
    }
    // continue
    if (get(loc) == CONTINUETK) {
        // m
        if (loop_layer <= 0) {
            loop_layer = 0;
            push_error("m", loc);
        }
        s = get_next();
        node.insert(s);
        if (get(loc) == SEMICN) {
            s = get_next();
            node.insert(s);
        }
        else {
            // i
            push_error("i", loc - 1);
        }
        return node;
    }
    // printf
    if (get(loc) == PRINTFTK) {
        int print_temp = loc;
        s = get_next();
        node.insert(s);
        if (get(loc) == LPARENT) {
            s = get_next();
            node.insert(s);
            if (get(loc) == STRCON) {
                int cnt = illegalSymbol();
                if (cnt == -1) {
                    // a
                    push_error("a", loc);
                }

                s = get_next();
                node.insert(s);

                int cnt_temp = 0;
                while (get(loc) == COMMA) {
                    s = get_next();
                    node.insert(s);
                    s = Expgr();
                    if (s.iserror()) {
                        return error();
                    }
                    node.insert(s);
                    cnt_temp++;
                }
                if (cnt != -1 && cnt != cnt_temp) {
                    // l
                    push_error("l", print_temp);
                }

                if (get(loc) == RPARENT) {
                    s = get_next();
                    node.insert(s);

                }
                // j
                else {
                    push_error("j", loc - 1);
                }
                if (get(loc) == SEMICN) {
                    s = get_next();
                    node.insert(s);
                }
                else {
                    // i
                    push_error("i", loc - 1);
                }
                return node;
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

            }
            // j
            else {
                push_error("j", loc - 1);
            }
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
            }
            // j
            else {
                push_error("j", loc - 1);
            }
            loop_layer++;

            s = Stmtgr();

            loop_layer--;

            if (!s.iserror()) {
                node.insert(s);
                return node;
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
        else {
            // i
            push_error("i", loc - 1);
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
        string name = l[loc].getWord();
        // c
        if (!symbols.top()->find(name)) {
            push_error("c", loc);
        }
        syntax s = get_next();
        node.insert(s);
        int dim = 0;
        while (get(loc) == LBRACK) {
            s = get_next();
            node.insert(s);

            dim++;
            s = Expgr();
            if (s.iserror()) {
                return error();
            }
            node.insert(s);
            if (get(loc) == RBRACK) {
                s = get_next();
                node.insert(s);
            }
            // k
            else {
                push_error("k", loc - 1);
            }
        }
        syntax *syn = &node.child[0];
        syn->dim = dim;
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
        int wrong_line = loc;
        string name = l[loc].getWord();
        int flag = 1;
        symbol *sym = nullptr;
        // c
        if (!symbols.top()->find(name)) {
            push_error("c", loc);
            flag = 0;
        }
        else {
            sym = symbols.top()->get(name);

        }
        vector<symbol> syms = sym->param;
        s = get_next();
        node.insert(s);
        if (get(loc) == LPARENT) {
            s = get_next();
            node.insert(s);
            s = FuncRParamsgr();

            if (!s.iserror()) {
                node.insert(s);
            }

            if (flag) {
                // d
                if (symbols.top()->get(name)->params != s.count_params()) {
                    push_error("d", wrong_line);
                }
                // e
                else {
                    // todo
                    vector<int> dims;
                    bool is_true = true;
                    for (syntax sy : s.child) {
                        if (sy.syn_type == Exp) {
                            pair<int, bool> p = find_in_exp(&sy);
                            dims.push_back(p.first);
                            if (!p.second) {
                                is_true = false;
                            }
                        }
                    }
                    for (int i = 0;i < syms.size(); i++) {
//                        cout<<dims[i]<<" "<<syms[i].dim<<endl;
                        if (dims[i] != syms[i].dim) {
                            is_true = false;
                        }
                    }
                    if (!is_true) {
                        push_error("e", wrong_line);
                    }
                }
            }


            if (get(loc) == RPARENT) {
                s = get_next();
                node.insert(s);
            }
            // j
            else {
                push_error("j", loc - 1);
            }
            return node;
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
void errorAnalysis()
{
//    error_output();
//    cout << symbols.toString() << endl;
}
void push_error(string type, int lo)
{
    errorType err(type, l[lo].getLine());
    errors.push_back(err);
}
void error_output()
{
    ofstream err_out;
    err_out.open("error.txt");
    sort(errors.begin(), errors.end());
    auto iter = unique(errors.begin(), errors.end());
    for(auto i = errors.begin(); i < iter; i++) {
        err_out << i->toString() << endl;
    }
}
int illegalSymbol()
{
    string s = l[loc].getWord();
    int cnt = 0;
    for(int i = 1; i < s.size() - 1; i++) {
        if (s[i] == '%') {
            if (s[i + 1] != 'd') {
                return -1;
            }
            else {
                i++;
                cnt++;
                continue;
            }
        }
        if (s[i] == '\\') {
            if ( s[i + 1] != 'n') {
                return -1;
            }
            i++;
            continue;
        }
        if (!(s[i] == 32 || s[i] == 33 ||
            (s[i] <= 126 && s[i] >= 40))) {
            return -1;
        }
    }
    return cnt;
}
pair<int, bool> find_in_exp(syntax* s)
{
    int dim = 0;
    bool return_value = true;
    syntax* t = s;
    while(t->syn_type != UnaryExp && !t->child.empty()) {
        t = &(t->child[0]);
    }
    if (t->child[0].flag == 0) {
        string ident = t->child[0].s;
        if(symbols.top()->get(ident)->type == "void") {
            return_value = false;
        }
    }
    else {
        while (t->syn_type != PrimaryExp
               && !t->child.empty()) {
            t = &t->child[t->child.size()-1];
        }
        if (t->child[0].category == LPARENT) {
            return find_in_exp(&t->child[1]);
        }
        if (t->child[0].syn_type == LVal) {
            t = &t->child[0];
            string name = t->child[0].s;
            if (!symbols.top()->find(name)) {
                return_value = false;
            }
            else {
                dim = symbols.top()->get(name)->dim;
                for(syntax sy : t->child) {
                    if(sy.category == LBRACK) {
                        dim--;
                    }
                }
            }
        }
    }
    return pair<int, bool>{dim, return_value};
}