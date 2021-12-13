//
// Created by 张瑞轩 on 2021/11/9.
//

#include "../include/interpreter.h"

stack<int> st;
stack<int> lines;
stack<int*> arrays;
stack<int> len;
int _loc = 0;
bool running = false;
stacks mem;
memory *basic;
ofstream _out;
map<string,int> m = {{"push", 1},
                     {"pop", 2},
                     {"add", 3},
                     {"sub", 4},
                     {"mul", 5},
                     {"div", 6},
                     {"mod", 7},
                     {"lss", 8},
                     {"leq",9},
                     {"eq", 10},
                     {"neq",11},
                     {"gt",12},
                     {"gte",13},
                     {"read",14},
                     {"print",15},
                     {"end",16},
                     {"init",17},
                     {"storei",18},
                     {"ldi",19},
                     {"jump",20},
                     {"jal",21},
                     {"copy",22},
                     {"bez",23},
                     {"bnz",24},
                     {"jback",25},
                     {"cb",26},
                     {"db",27},
                     {"cnb",28},
                     {"dnb",29},
                     {"land",30},
                     {"lor",31},
                     {"ops",32},
                     {"not",33},
                     {"clear",34},
                     {"off",35},
                     {"init2",36},
                     {"off2",37}};
map<string,int> label2instr;
map<string,my_array> id2array;


void run(vector<instr> ve)
{
    _out.open("pcoderesult.txt");
    basic = mem.top()->top();

    running = true;
    int len = ve.size();
//    for (instr i : ve) {
//        cout<<i.tostring()<<endl;
//    }
//    cout<<endl;
    for (int i = 0;i<ve.size();i++) {
        if (ve[i].flag == 1) {
            if (label2instr.count(ve[i].name) == 0) {
                label2instr[ve[i].name] = i;
            }
        }
    }
    while(running && _loc < len) {
        instr ins = ve[_loc];
        if (ins.flag == 1) {
            _loc++;
            continue;
        }
        if (m.count(ins.name) < 1) {
            cout<<"error instr \"" << ins.name <<"\" at line "<< _loc << endl;
            running = false;
        }
        else {
            switch (m[ins.name]) {
                // push
                case 1:             // push
                    push(ins.x);
                    _loc++;
                    break;
                case 2:             // pop
                    pop();
                    _loc++;
                    break;
                case 3:             // add
                    add();
                    _loc++;
                    break;
                case 4:             // sub
                    sub();
                    _loc++;
                    break;
                case 5:             // mul
                    mul();
                    _loc++;
                    break;
                case 6:             // div
                    div();
                    _loc++;
                    break;
                case 7:             // mod
                    mod();
                    _loc++;
                    break;
                case 8:			    // lss
                    lss();
                    _loc++;
                    break;
                case 9:			    // leq
                    leq();
                    _loc++;
                    break;
                case 10:			// eq
                    eq();
                    _loc++;
                    break;
                case 11:			// neq
                    neq();
                    _loc++;
                    break;
                case 12:			// gt
                    gt();
                    _loc++;
                    break;
                case 13:			// gte
                    gte();
                    _loc++;
                    break;
                case 14:			// read
                    read();
                    _loc++;
                    break;
                case 15:			// print
                    if (!ins.y.empty()) {
                        print(ins.y);
                    }
                    else {

                        print();
                    }
                    _loc++;
                    break;
                case 16:			// end
                    running = false;
                    break;
                case 17:			// init
                    init(ins.y);
                    _loc++;
                    break;
                case 18:			// storei
                    storei(ins.y);
                    _loc++;
                    break;
                case 19:			// ldi
                    ldi(ins.y);
                    _loc++;
                    break;
                case 20:			// jump
                    jump(ins.y);
                    break;
                case 21:			// jal
                    jal(ins.y);
                    break;
                case 22:			// copy
                    copy();
                    _loc++;
                    break;
                case 23:			// bez
                    bez(ins.y);
                    break;
                case 24:			// bnz
                    bnz(ins.y);
                    break;
                case 25:			// jback
                    jback();
                    break;
                case 26:			// cb
                    cb();
                    _loc++;
                    break;
                case 27:			// db
                    db();
                    _loc++;
                    break;
                case 28:			// cnb
                    cnb();
                    _loc++;
                    break;
                case 29:			// dnb
                    dnb();
                    _loc++;
                    break;
                case 30:			// land
                    land();
                    _loc++;
                    break;
                case 31:			// lor
                    lor();
                    _loc++;
                    break;
                case 32:			// ops
                    ops();
                    _loc++;
                    break;
                case 33:			// not
                    no();
                    _loc++;
                    break;
                case 34:			// clear
                    clear(ins.x);
                    _loc++;
                    break;
                case 35:			// off
                    off(ins.y, ins.x);
                    _loc++;
                    break;
                case 36:			// init2
                    init2(ins.y);
                    _loc++;
                    break;
                case 37:			// off2
                    off2(ins.y, ins.x);
                    _loc++;
                    break;
                default:
                    break;
            }
        }
//        cout<<ins.name<<endl;
//        cout<<mem.tostring()<<"****************************************"<<endl;
    }
}

void push(int num)
{
    st.push(num);
}
void pop()
{
    st.pop();
}
void add()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a+b);
}
void sub()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a-b);
}
void mul()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a*b);
}
void div()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a/b);
}
void mod()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a%b);
}
void lss()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a<b);
}
void leq()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a<=b);
}
void eq()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a==b);
}
void neq()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a!=b);
}
void gt()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a>b);
}
void gte()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a>=b);
}
void read()
{
    int x;
    cin>>x;
    st.push(x);
}
void print()
{
    int num = st.top();
    st.pop();
    _out<<num;
}
void print(string s)
{
    _out<<s;
}
void init(string s)
{
    int div = st.top();
    st.pop();
    my_array arr(s);
    arr.div = div;
    int length = 1;
    for (int i = 0; i < div; i++) {
        int l = st.top();
        st.pop();
        arr.push(l);
        length *= l;
    }
    arr.count();
    arr.init();
    mem.top()->add_array(arr);
//    mem.top()->init(s, length);
}
void init2(string s)
{
    int div = st.top();
    st.pop();
    my_array arr(s);
    arr.div = div;
    int length = 1;
    for (int i = 1; i < div; i++) {
        int l = st.top();
        st.pop();
        arr.push(l);
        length *= l;
    }
    int *v = arrays.top();
    int size = len.top();
    arrays.pop();
    len.pop();
    arr.push(size/length);
    arr.set_array(v);
    int off = st.top();

    st.pop();
    arr.count();
    mem.top()->add_array(arr);
//    mem.top()->init(s, size);

    for (int i = 0;i < size; i++) {
        mem.top()->assign(s, i, v[i]);
    }
}
void storei(string s)
{
    int off = st.top();
    st.pop();
    int x = st.top();
    st.pop();
    mem.top()->assign(s, off, x);
}
void ldi(string s)
{
    int off = st.top();
    st.pop();

    int x = mem.top()->get(s, off);
    st.push(x);
}
void jump(string s)
{
    if (label2instr.count(s) == 0) {
        _loc++;
        return;
    }
    int instr = label2instr[s];
    _loc = instr;
}
void jal(string s)
{
    if (label2instr.count(s) == 0) {
        _loc++;
        return;
    }
    int instr = label2instr[s];
    lines.push(_loc + 1);
    _loc = instr;
}
void copy()
{
    int x = st.top();
    st.push(x);
}
void bez(string s)
{
    int x = st.top();
    st.pop();
    if (label2instr.count(s) == 0) {
        _loc++;
        return;
    }
    int instr = label2instr[s];
    if (x == 0) {
        _loc = instr;
    }
    else {
        _loc++;
    }
}
void bnz(string s)
{
    int x = st.top();
    st.pop();
    if (label2instr.count(s) == 0) {
        _loc++;
        return;
    }
    int instr = label2instr[s];
    if (x != 0) {
        _loc = instr;
    }
    else {
        _loc++;
    }
}
void jback()
{
    int x = lines.top();
    lines.pop();
    _loc = x;
}
void cb()
{
    mem.top()->add();
}
void db()
{
    mem.top()->pop();
}
void cnb()
{
    *basic = *mem.top()->basic;
    mem.add(basic);
    mem.top()->add();
}
void dnb()
{
    *basic = mem.pop();
    *(mem.top()->basic) = *basic;
}
void land()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a&&b);
}
void lor()
{
    int b = st.top();
    st.pop();
    int a = st.top();
    st.pop();
    st.push(a||b);
}
void ops()
{
    int x = st.top();
    st.pop();
    st.push(-x);
}
void no()
{
    int x = st.top();
    st.pop();
    st.push(!x);
}
void clear(int flag)
{
    int value = 0;
    if (flag == 1) {
        value = st.top();
        st.pop();
    }
    while (!st.empty()) {
        int x = st.top();
        st.pop();
        if (x == baseline) {
            break;
        }
    }
    if (flag == 1) {
        st.push(value);
    }
}
void off(string s, int div)
{
    int sum = 0;
    int div_real = mem.top()->get_div(s);
    for (int i = div_real - div; i < div_real; i ++) {
        int x = st.top();
        st.pop();
        sum += x * mem.top()->get_n(s, i);
    }
    if (div_real > div) {
        len.push(mem.top()->get_n(s, div_real - div));
        arrays.push(mem.top()->get_array(s, sum));
//        arrays.push(mem.top()->get_mem(s, sum,
//                           mem.top()->get_n(s, div_real - div)));
//        for (int i : arrays.top())
//            cout<<i<<" ";
    }

    st.push(sum);
}
void off2(string s, int div)
{
    int sum = 0;
    int div_real = mem.top()->get_div(s);
    for (int i = div_real - div; i < div_real; i ++) {
        int x = st.top();
        st.pop();
        sum += x * mem.top()->get_n(s, i);
    }
    if (div_real > div) {
        len.push(mem.top()->get_n(s, div_real - div));
        arrays.push(mem.top()->get_array(s, sum));
//        arrays.push(mem.top()->get_mem(s, sum,
//                                       mem.top()->get_n(s, div_real - div)));
//        for (int i : arrays.top())
//            cout<<i<<" ";
//        cout<<sum<<endl;
        st.push(sum);
    }
    else {
        st.push(sum);
        ldi(s);
    }
}