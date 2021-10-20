//
// Created by 张瑞轩 on 2021/10/18.
//

#include "../include/errorType.h"

errorType::errorType(string type, int line) {
    this->type = type;
    this->line = line;
}

string errorType::toString() {
    return to_string(line) + " " + type;
}

bool errorType::operator<(const errorType &err) const {
    return line < err.line;
}

bool errorType::operator>(const errorType &err) const {
    return line > err.line;
}

bool errorType::operator==(const errorType &err) const {
    return line == err.line;
}
