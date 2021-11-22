//
// Created by 张瑞轩 on 2021/11/9.
//

#include "../include/instr.h"

instr::instr() {
}

instr::instr(string name) {
    this->name = name;
}

instr::instr(string name, int x) {
    this->name = name;
    this->x = x;
}

instr::instr(string name, string y) {
    this->name = name;
    this->y = y;
}

instr::instr(int flag, string name) {
    this->name = name;
    this->flag = flag;
}

string instr::tostring() {
    string s;
    s.append(name);
    if (flag == 1) {
        s.append(":");
    }
    else if (y != "") {
        s.append(" " + y);
    }
    else if (x != 0 || name == "push") {
        s.append(" " + to_string(x));
    }
    return s;
}

instr::instr(string name, int x, string y) {
    this->name = name;
    this->y = y;
    this->x = x;
}
