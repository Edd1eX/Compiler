//
// Created by 张瑞轩 on 2021/11/10.
//

#include "../include/memory.h"


int memory::find(string name) {
    if (id2array.count(name) == 0) {
        return 0;
    }
    return 1;
}

int memory::get(string name, int off) {
    if (find(name)) {
        return id2array[name].get_val(off);
    }
    return -1;
}

void memory::assign(string name, int off, int val) {
    if (find(name)) {
        id2array[name].assign(off, val);
    }
}

memory::memory() {

}

string memory::tostring() {
    string s;
    for(auto & iter : id2array) {
        s.append(iter.second.tostring() + "\n");
    }
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

int *memory::get_array(string name, int off) {
    if (id2array.count(name) > 0) {
        return id2array[name].get_array(off);
    }
    else if (father != nullptr) {
        return father->get_array(name, off);
    }
    return nullptr;
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
        if(loc == 0) {
            p = p->father;
            continue;
        }
        p->assign(name, off, x);
        break;
    }
}

int memoryStack::get(string name, int off) {
    memory *p = &memories.top();
    while(p!= nullptr) {
        int loc = p->find(name);
        if(loc == 0) {
            p = p->father;
            continue;
        }
        return p->get(name, off);
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

void memoryStack::add_array(my_array arr) {
    top()->add_array(arr);
}

int memoryStack::get_div(string name) {
    return top()->get_div(name);
}

int memoryStack::get_n(string name, int n) {
    return top()->get_n(name, n);
}

int *memoryStack::get_array(string name, int off) {
    return top()->get_array(name, off);
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

my_array::my_array() {

}

void my_array::count() {
    int t = 1;
    res.push_back(t);
    for (int i = 0 ;i < div; i ++) {
        t *= params[i];
        res.push_back(t);
    }
    length = t;
}

void my_array::init() {
    if (div > 0) {
        arr = (int *)malloc(sizeof(int) * length);
        for (int i = 0;i < length; i++) {
            arr[i] = 0;
        }
    }
    else {
        num = 0;
    }
}

void my_array::assign(int loc, int val) {
    if (loc < length) {
        if (div == 0) {
            num = val;
        }
        else {
            arr[loc] = val;
        }
    }
}

int my_array::get_val(int loc) {
    if (loc < length) {
        if (div == 0) {
            return num;
        }
        else {
            return arr[loc];
        }
    }
    return 0;
}

int *my_array::get_array(int loc) {
    if (loc < length && div > 0) {
        return arr + loc;
    }
    return nullptr;
}

void my_array::set_array(int *arr) {
    this->arr = arr;
}

string my_array::tostring() {
    string s = "";
    s += name + ": ";
    if (div == 0) {
        s += to_string(num);
    }
    else {
        for (int i = 0; i < length; i++) {
            if (i == 0) {
                s += to_string(arr[i]);
            }
            else {
                s += ' ' + to_string(arr[i]);
            }
        }
    }
    return s;
}
