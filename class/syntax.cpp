//
// Created by 张瑞轩 on 2021/10/16.
//

#include "../include/syntax.h"

syntax::syntax(word w) {
    this->s = w.getWord();
    this->flag = 0;
    this->line = w.getLine();
    this->category = w.getCategory();
}

syntax::syntax(string syn_type) {
    this->syn_type = syn_type;
    this->flag = 1;
}

void syntax::insert(syntax c) {
    this->child.push_back(c);
}

syntax::syntax(int error) {
    this->flag = error;
}

bool syntax::iserror() {
    return this->flag == -1;
}

string syntax::getName() {
    if (flag == 0 && category == "IDENFR") {
        return s;
    }
    if (!child.empty()) {
        for (syntax s : child) {
            if (s.flag == 0 && s.category == "IDENFR") {
                return s.s;
            }
            string name = s.getName();
            if (name != "") {
                return name;
            }
        }
    }
    return "";
}

string syntax::getType() {
    if (flag == 0 && (category == "INTTK" || category == "VOIDTK" )) {
        return s;
    }
    if (!child.empty()) {
        for (syntax s : child) {
            if (s.flag == 0 && (s.category == "INTTK" || s.category == "VOIDTK" )) {
                return s.s;
            }
            string name = s.getName();
            if (name != "") {
                return name;
            }
        }
    }
    return "";
}

bool syntax::checkLast() {
    syntax *t = this;
    while (!t->child.empty() && t->syn_type != Stmt) {
        t = &(t->child).back();
    }
    if (t->syn_type != Stmt) {
        return false;
    }
    if (t->child.size() >= 2 && t->child[0].category == RETURNTK
                            && t->child[1].syn_type == Exp) {
        return true;
    }
    return false;
}

int syntax::count_params() {
    if (syn_type != FuncRParams) {
        return 0;
    }
    int cnt = 0;
    for (syntax s : child) {
        if (s.syn_type == Exp) {
            cnt++;
        }
    }
    return cnt;
}

vector<symbol> syntax::get_param() {
    vector<symbol> syms;
    if (syn_type != FuncFParams) {
        return syms;
    }
    for (syntax s : child) {
        if (s.syn_type == FuncFParam) {
            string name = s.child[1].getName();
            string type = s.child[0].getType();
            int dim = 0;
            for(int i = 2;i<s.child.size();i++) {
                if (s.child[i].category == LBRACK) {
                    dim++;
                }
            }
            syms.emplace_back(dim,name,"var",type);
        }
    }
    return syms;
}

