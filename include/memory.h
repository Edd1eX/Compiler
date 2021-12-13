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
    int length = 1;
    int num;
    int *arr;
    vector<int> params;
    vector<int> res;

    my_array();
    my_array(string name);
    void push(int n);
    void count();
    void init();
    void assign(int off, int val);
    int get_val(int off);
    int *get_array(int off);
    void set_array(int *arr);
    int get(int n);
    string tostring();
};

class memory {
public:
    map<string,my_array> id2array;
    memory *father = nullptr;

    memory();
    void add_array(my_array);
    int find(string name);
    int get(string name, int off);
    int get_div(string name);
    int get_n(string name, int n);
    int *get_array(string name, int off);
    void assign(string name, int off, int val);
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
    void add_array(my_array);
    void assign(string name, int off,int val);
    int get(string name, int off);
    int get_n(string name, int n);
    int get_div(string name);
    int *get_array(string name, int off);
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
