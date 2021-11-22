//
// Created by 张瑞轩 on 2021/11/10.
//
#include<bits/stdc++.h>
using namespace std;
#ifndef COMPILER_MEMORY_H
#define COMPILER_MEMORY_H

class my_array {
public:
    string name;
    int div = 0;
    vector<int> params;
    vector<int> res;

    my_array();
    my_array(string name);
    void push(int n);
    void count();
    int get(int n);
    int sum();
};

class memory {
public:
    vector<int> mem;
    map<string,int> id2mem;
    map<string,my_array> id2array;
    memory *father = nullptr;

    memory();
    void init(string name);
    void init(string name,int length);
    void add_array(my_array);
    int find(string name);
    int get(int loc);
    int get_div(string name);
    int get_n(string name, int n);
    vector<int> get_mem(string name, int off, int length);
    void assign(int loc, int x);
    string tostring();
};

class memoryStack {
public:
    stack<memory> memories;
    memory *basic;

    memoryStack(memory*);
    void add();
    void pop();
    memory* top();
    void init(string name);
    void init(string name,int length);
    void add_array(my_array);
    void assign(string name, int off,int x);
    int get(string name, int off);
    int get_n(string name, int n);
    int get_div(string name);
    vector<int> get_mem(string name, int off, int length);
    string tostring();
};

class stacks {
public:
    stack<memoryStack> st;

    stacks();
    memoryStack* top();
    void add(memory* basic);
    memory pop();
    string tostring();
};


#endif //COMPILER_MEMORY_H
