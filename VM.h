#ifndef VM_H
#define VM_H

#include "main.h"


enum Type{ is_int, is_real, is_bool, is_addr, is_reg , is_non};
enum Code {
    Add, //0
    Minus,
    Mul, //2
    Div,
    CmpEQ,//4
    CmpNE,
    CmpLT,//6
    CmpLE,
    CmpGT,//8
    CmpGE,
    Not,//10
    And,
    Or,//12
    Move,
    Load,//14
    Store,
    Jump,//16
    JumpIf,
    Call,//18
    Return,
    Halt,//20
    Input,
    Output,//22
    Non
};


struct Node {
    int val;
    Node*next;
public:
    Node(int val) {
        this->val = val;
        this->next = NULL;
    }
};
class Stack {
    Node* head;
    Node* temp;
    Node* iter;
    
    int num;
public:
    Stack() {
        head = temp = iter = NULL;
        num = 0;
    }
    ~Stack() {
        iter = head;
        while(iter) {
            temp = iter;
            iter = iter->next;
            delete temp;
        }
    }
    void push(int);
    int pop();
    int top();
    bool isEmpty();
    bool isFull();
    void print();//for debug
};


class Data{
    Type type;
    union {
        int ival;
        double rval;
    } val;
public:
    Data() {
        type = is_non;
        val.ival = 0;
    }
    friend class VM;
};

class Src{
    Type type;
    union {
        int ival;
        double rval;
    } val;
public:
    Src() {
        type = is_non;
        val.ival = 0;
    }
    friend class VM;
};
class CMD {
    Code code;
    int reg;
    Src src;
    friend class VM;
public:
    CMD() {
        code = Non;
        reg = 0;
    }
} ;



class VM {
    int numCMD; // number of cmd in memmory bank
    CMD *memBank;
    Stack reAdStack;
    Data *staticMem;
    Data regis[15];
    int IP;
public:
    VM(){
        numCMD = 0;
        memBank = NULL;
        IP = 0;
        staticMem = new Data[65536];
    }
    ~VM(){
        if(memBank) {
            delete []memBank;
        }
        if(staticMem) {
            delete []staticMem;
        }
    }
    void run(string);
    
    void compile_error(string);
    bool instruction_error(const string &);
    bool operand_error(const string &);
    void storeCMD(const string&,int);
    void extract2op(const string&, int);
    void extract1op(const string&, int, bool);
    
    void readCMD();
    void executeCMD(bool&);
    
    int& getSrcInt(int i) {
        if(memBank[i].src.type == is_reg) {
            return regis[memBank[i].src.val.ival-1].val.ival;
        }
        return memBank[i].src.val.ival;
    }
    double& getSrcReal(int i) {
        if(memBank[i].src.type == is_reg) {
            return regis[memBank[i].src.val.ival-1].val.rval;
        }
        return memBank[i].src.val.rval;
    }
    int& getDestInt(int i) {
        return regis[memBank[i].reg-1].val.ival;
    }
    double& getDestReal(int i) {
        return regis[memBank[i].reg-1].val.rval;
    }
    Type& getDestType(int i) {
        return regis[memBank[i].reg-1].type;
    }
    Type& getSrcType(int i) {
        if(memBank[i].src.type == is_reg) {
            return regis[memBank[i].src.val.ival-1].type;
        }
        return memBank[i].src.type;
    }
    
    
    void arithmetic();
    void compare();
    void MoveData();
    void StoreData();
    void LoadData();
    void OutputData();
    void InputData();
    
    void printCMD(); // for debug
    void printReg();//for debug
};
#endif
