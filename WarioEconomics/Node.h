//
// Created by Joseph on 10/27/2020.
//

#ifndef WARIOECONOMICS_NODE_H
#define WARIOECONOMICS_NODE_H

#include <iostream>

enum TrigFunction{None=0,Sin=1,Cos=2,Tan=3,Sec=4};

class Node {

private:
    double outer = 0;
    double inner = 0;
    int num = 0;
    int denom = 0;
    int exp = 0;
    TrigFunction trig = None;
    Node* next = nullptr;

public:
    Node(double outer, int exp);
    Node(double outerIn, double innerIn, TrigFunction trigIn);
    Node(double inner, int num, int denom, int exp);
    Node(const Node& to_copy);

    friend std::ostream& operator<<(std::ostream& os, const Node& node);

    void copy(const Node& to_copy);
    void differentiate();
    bool hasNext() const;
    bool usesTrig() const;
    std::string getDelimiter() const;

    double getOuter() const;
    void setOuter(double outer);
    double getInner() const;
    void setInner(double inner);
    int getNum() const;
    void setNum(int num);
    int getDenom() const;
    void setDenom(int denom);
    int getExp() const;
    void setExp(int exp);
    int getTrig() const;
    void setTrig(TrigFunction trigIn);
    Node* getNext() const;
    void setNext(Node* next);

};

#endif //WARIOECONOMICS_NODE_H