//
// Created by 张瑞轩 on 2021/9/23.
//

#include "lexicalAnalysis.h"

vector <char> str;
vector <word> lexical;
map<string,string> keyword = {{"return", RETURNTK},
                              {"main", MAINTK},
                              {"const", CONSTTK},
                              {"int", INTTK},
                              {"break", BREAKTK},
                              {"continue", CONTINUETK},
                              {"if", IFTK},
                              {"else", ELSETK},
                              {"while",WHILETK},
                              {"getint",GETINTTK},
                              {"printf",PRINTFTK},
                              {"void",VOIDTK}};
int line = 1;
void analysis()
{
    getText();
    go();
    output();
}

void getText()
{
    FILE *in = fopen("testfile.txt", "r");
    char s[1000];

    while (fgets(s,1000,in) != NULL) {
        for (int i = 0; i < strlen(s); i++)
            str.push_back(s[i]);
    }
}

void output()
{
    ofstream out;
    out.open("output.txt");
    for (word w : lexical) {
        out << w.getCategory() + " " + w.getWord() << endl;
//        out << w.getCategory() + " " + w.getWord() + " " + to_string(w.getLine()) << endl;
    }
//    for(char i : str) {
//        out<<i;
//    }
    out.close();
}

void go()
{
    int now = 0;
    while (now < str.size()) {
        if (str[now] == '\n') {
            line++;
        }
        if (str[now] <= 32) {
            now++;
            continue;
        }

        bool isSpecial = 1;
        switch (str[now]) {
            case '(':
                now = LPARENTr(now);
                break;
            case ')':
                now = RPARENTr(now);
                break;
            case '[':
                now = LBRACKr(now);
                break;
            case ']':
                now = RBRACKr(now);
                break;
            case '{':
                now = LBRACEr(now);
                break;
            case '}':
                now = RBRACEr(now);
                break;
            case ';':
                now = SEMICNr(now);
                break;
            case ',':
                now = COMMAr(now);
                break;
            case '=':
                now = ASSIGNr(now);
                break;
            case '<':
                now = LSSr(now);
                break;
            case '>':
                now = GREr(now);
                break;
            case '!':
                now = NOTr(now);
                break;
            case '+':
                now = PLUSr(now);
                break;
            case '-':
                now = MINUr(now);
                break;
            case '*':
                now = MULTr(now);
                break;
            case '/':
                now = DIVr(now);
                break;
            case '%':
                now = MODr(now);
                break;
            case '&':
                now = ANDr(now);
                break;
            case '|':
                now = ORr(now);
                break;
            case '"':
                now = STRCONr(now);
                break;
            default:
                isSpecial = false;
                break;
        }
        if(isSpecial) {
            continue;
        }
        if(isdigit(str[now])) {
            now = INTCONr(now);
        }
        else{
            now = WORDr(now);
        }
    }
}


int INTCONr(int loc)
{
    int len = 0;
    string s = "";
    while(isdigit(str[loc + len])) {
        s.push_back(str[loc + len]);
        len++;
    }
    word w(s, INTCON, line);
    lexical.push_back(w);
    return loc + len;
}

int STRCONr(int loc)
{
    int len = 1;
    string s = "\"";
    while(str[loc + len] != '\"') {
        s.push_back(str[loc + len]);
        len++;
    }
    s.push_back('\"');
    len++;
    word w(s, STRCON, line);
    lexical.push_back(w);
    return loc + len;
}

int NOTr(int loc)
{
    if (str[loc + 1] == '=') {
        return NEQr(loc);
    }
    word w("!", NOT, line);
    lexical.push_back(w);
    return loc + 1;

}
int ANDr(int loc)
{
    word w("&&", AND, line);
    lexical.push_back(w);
    return loc + 2;
}
int ORr(int loc)
{
    word w("||", OR, line);
    lexical.push_back(w);
    return loc + 2;
}

int PLUSr(int loc)
{
    word w("+", PLUS, line);
    lexical.push_back(w);
    return loc + 1;
}

int MINUr(int loc)
{
    word w("-", MINU, line);
    lexical.push_back(w);
    return loc + 1;
}

int MULTr(int loc)
{
    word w("*", MULT, line);
    lexical.push_back(w);
    return loc + 1;
}

int DIVr(int loc)
{
    // TODO
    // 注释
    if (str[loc + 1] == '/') {
        return skipComment1(loc + 2);
    }
    else if (str[loc + 1] == '*') {
        return skipComment2(loc + 2);
    }
    word w("/", DIV, line);
    lexical.push_back(w);
    return loc + 1;
}

int MODr(int loc)
{
    word w("%", MOD, line);
    lexical.push_back(w);
    return loc + 1;
}

int LSSr(int loc)
{
    if (str[loc + 1] == '=') {
        return LEQr(loc);
    }
    word w("<", LSS, line);
    lexical.push_back(w);
    return loc + 1;
}

int LEQr(int loc)
{
    word w("<=", LEQ, line);
    lexical.push_back(w);
    return loc + 2;
}

int GREr(int loc)
{
    if (str[loc + 1] == '=') {
        return GEQr(loc);
    }
    word w(">", GRE, line);
    lexical.push_back(w);
    return loc + 1;
}

int GEQr(int loc)
{
    word w(">=", GEQ, line);
    lexical.push_back(w);
    return loc + 2;
}

int EQLr(int loc)
{
    word w("==", EQL, line);
    lexical.push_back(w);
    return loc + 2;
}

int NEQr(int loc)
{
    word w("!=", NEQ, line);
    lexical.push_back(w);
    return loc + 2;
}

int ASSIGNr(int loc)
{
    if (str[loc + 1] == '=') {
        return EQLr(loc);
    }
    word w("=", ASSIGN, line);
    lexical.push_back(w);
    return loc + 1;
}

int SEMICNr(int loc)
{
    word w(";", SEMICN, line);
    lexical.push_back(w);
    return loc + 1;
}

int COMMAr(int loc)
{
    word w(",", COMMA, line);
    lexical.push_back(w);
    return loc + 1;
}

int LPARENTr(int loc)
{
    word w("(", LPARENT, line);
    lexical.push_back(w);
    return loc + 1;
}

int RPARENTr(int loc)
{
    word w(")", RPARENT, line);
    lexical.push_back(w);
    return loc + 1;
}

int LBRACKr(int loc)
{
    word w("[", LBRACK, line);
    lexical.push_back(w);
    return loc + 1;
}

int RBRACKr(int loc)
{
    word w("]", RBRACK, line);
    lexical.push_back(w);
    return loc + 1;
}

int LBRACEr(int loc)
{
    word w("{", LBRACE, line);
    lexical.push_back(w);
    return loc + 1;
}

int RBRACEr(int loc)
{
    word w("}", RBRACE, line);
    lexical.push_back(w);
    return loc + 1;
}

int WORDr(int loc)
{
    string s = "";
    int len = 0;
    while(isalpha(str[loc + len]) || str[loc + len] == '_' || (isdigit(str[loc + len]) && len > 0)) {
        s.push_back(str[loc + len]);
        len++;
    }
    if (keyword.count(s) > 0) {
        word w(s, keyword[s], line);
        lexical.push_back(w);
    }
    else {
        word w(s, IDENFR, line);
        lexical.push_back(w);
    }
    return loc + len;
}

int skipComment1(int loc)
{
    int len = 0;
    while (str[loc + len] != '\n' || loc + len > str.size()) {
        len++;
    }
    line++;
    return loc + len + 1;
}

int skipComment2(int loc)
{
    int len = 0;
    while (str[loc + len] != '*' || str[loc + len + 1] != '/') {
        if (str[loc + len] == '\n') {
            line++;
        }
        len++;
    }
    return loc + len + 2;
}
