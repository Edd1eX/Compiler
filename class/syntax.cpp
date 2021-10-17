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

