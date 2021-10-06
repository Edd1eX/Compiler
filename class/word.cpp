////
//// Created by 张瑞轩 on 2021/9/23.
////
//
#include "../include/word.h"

word::word (string s, string category, int line) {
    this->s = s;
    this->category = category;
    this->line = line;
}

string word::getWord() {
    return s;
}

string word::getCategory() {
    return category;
}

int word::getLine() {
    return line;
}