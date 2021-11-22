//
// Created by 张瑞轩 on 2021/11/10.
//

#include "../include/memory.h"

void memory::init(string name) {
    if (id2mem.count(name) == 0) {
        int loc = mem.size();
        id2mem[name] = loc;
        mem.push_back(0);
    }
}

void memory::init(string name, int length) {
    if (id2mem.count(name) == 0) {
        int loc = mem.size();
        id2mem[name] = loc;
        for(int i = 0;i < length; i++) {
            mem.push_back(0);
        }
    }
}

int memory::find(string name) {
    if (id2mem.count(name) == 0) {
        return -1;
    }
    return id2mem[name];
}

int memory::get(int loc) {
    if (loc >= mem.size()) {
        return -1;
    }
    return mem[loc];
}

void memory::assign(int loc, int x) {
    vector<int> *v = &mem;
    if (loc < mem.size())
//        mem[loc] = x;
        v->at(loc) = x;
}

memory::memory() {

}

string memory::tostring() {
    string s;
    int num = 0;
    for (int n:mem) {
        s.append(to_string(n) + "  ");
        num++;
        if (num >= 10) {
            num%=10;
            s.append("\n");
        }
    }
    s.append("\n");
    return s;
}

void memory::add_array(my_array arr) {
    string name = arr.name;
    if (id2array.count(name) == 0) {
        id2array[name] = arr;
    }
}

int memory::get_div(string name) {
    if (id2array.count(name) > 0) {
        return id2array[name].div;
    }
    else if (father != nullptr) {
        return father->get_div(name);
    }
    return 0;
}

int memory::get_n(string name, int n) {
    if (id2array.count(name) > 0) {
        return id2array[name].get(n);
    }
    else if (father != nullptr) {
        return father->get_n(name, n);
    }
    return 0;
}

vector<int> memory::get_mem(string name, int off, int length) {
    vector<int> v;
    if (id2mem.count(name) > 0) {
        int loc = id2mem[name] + off;
        for (int i = 0; i < length; i++) {
            if (i + loc < mem.size()) {
                v.push_back(mem[i + loc]);
            }
        }
    }
    return v;
}

memoryStack::memoryStack(memory* basic) {
    memories.push(*basic);
    this->basic=&memories.top();
}

void memoryStack::add() {
    memory m;
    m.father=&memories.top();
    memories.push(m);
}

void memoryStack::assign(string name,int off, int x)
{
    memory *p = &memories.top();
    while(p!= nullptr) {
        int loc = p->find(name);
        if(loc == -1) {
            p = p->father;
            continue;
        }
        p->assign(loc + off, x);
        break;
    }
}

int memoryStack::get(string name, int off) {
    memory *p = &memories.top();
    while(p!= nullptr) {
        int loc = p->find(name);
        if(loc == -1) {
            p = p->father;
            continue;
        }
        return p->get(loc + off);
    }
    return -1;
}

void memoryStack::pop() {
    memories.pop();
}

memory *memoryStack::top() {
    return &memories.top();
}

string memoryStack::tostring() {
    string s;
    memory *m = top();
    s.append("-------------------------\n");
    while (m != nullptr) {
        s.append(m->tostring());
        s.append("-------------------------\n");
        m = m->father;
    }
    return s;
}

void memoryStack::init(string name) {
    top()->init(name);
}

void memoryStack::init(string name, int length) {
    top()->init(name,length);
}

void memoryStack::add_array(my_array arr) {
    top()->add_array(arr);
}

int memoryStack::get_div(string name) {
    return top()->get_div(name);
}

int memoryStack::get_n(string name, int n) {
    return top()->get_n(name, n);
}

vector<int> memoryStack::get_mem(string name, int off, int length) {
    memory *p = &memories.top();
    while(p!= nullptr) {
        vector<int> v = p->get_mem(name, off, length);
        if(v.empty()) {
            p = p->father;
            continue;
        }
        return v;
    }
    return vector<int>();
}

memoryStack* stacks::top() {
    return &st.top();
}

void stacks::add(memory* basic) {
    st.push(memoryStack(basic));
}

memory stacks::pop() {
    memory m = *top()->basic;
    st.pop();
    return m;
}

stacks::stacks()
{
    memory *p;
    memory m;
    p = &m;
    p->father = nullptr;
    st.push(memoryStack(p));
}

string stacks::tostring() {
    return top()->tostring();
}

my_array::my_array(string name) {
    this->name = name;
}

void my_array::push(int n) {
    params.push_back(n);
}

int my_array::get(int n) {
    if (n <= div) {
        return res[n];
    }
    return 0;
}

int my_array::sum() {
    if (div > 0) {
        int res = 1;
        for (int num : params) {
            res *= num;
        }
        return res;
    }
    return 0;
}

my_array::my_array() {

}

void my_array::count() {
    int t = 1;
    res.push_back(t);
    for (int i = 0 ;i < div; i ++) {
        t *= params[i];
        res.push_back(t);
    }
}
