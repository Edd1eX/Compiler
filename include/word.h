////
//// Created by 张瑞轩 on 2021/9/23.
////
#include<bits/stdc++.h>
using namespace std;

#ifndef COMPILER_WORD_H
#define COMPILER_WORD_H

class word
{
private:
    string s;
    string category;
    int line;

public:
    word (string s, string category,int line);
    string getWord();
    string getCategory();
    int getLine();
};


#endif //COMPILER_WORD_H
