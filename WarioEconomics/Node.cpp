//
// Created by Joseph on 10/27/2020.
//

#include "Node.h"

Node::Node(double outerIn, int expIn) {
    outer = outerIn;
    exp = expIn;
}

Node::Node(double outerIn, double innerIn, TrigFunction trigIn) {
    outer = outerIn;
    inner = innerIn;
    trig = trigIn;
    exp = -11;
}

Node::Node(const Node& to_copy) {
    copy(to_copy);
}

Node::Node(double innerIn, int numIn, int denomIn, int expIn) {
    inner = innerIn;
    num = numIn;
    denom = denomIn;
    exp = expIn;
}

void Node::copy(const Node &to_copy) {
    outer = to_copy.getOuter();
    inner = to_copy.getInner();
    num = to_copy.getNum();
    denom = to_copy.getDenom();
    exp = to_copy.getExp();
    trig = static_cast<TrigFunction>(to_copy.getTrig());
}

double abs(double num) {
    return num < 0 ? -num : num;
}

std::ostream& operator<<(std::ostream& os, const Node& node) {
    //exp
    int exp_value = node.getExp();
    std::string exp = exp_value != 0 && exp_value != 1 ? std::to_string(exp_value) : "";
    std::string x = "x^";
    if(exp_value == 0) x = "";
    else if(exp_value == 1) x = "x";
    //outer and inner values
    double inner_value = node.getInner();
    double outer_value = abs(node.getOuter());
    if(outer_value == 0) {
        os << "0";
        return os;
    }
    //determine what type of trig function it uses
    switch(node.getTrig()) {
        case None:
            if(outer_value != 1 || x.empty()) {
                os << outer_value << x << exp;
            } else {
                os << x << exp;
            }
            break;
        case Sin:
            if(inner_value != 1) {
                if(outer_value != 1) {
                    os << outer_value << "sin " << inner_value << "x";
                } else {
                    os << "sin " << inner_value << "x";
                }
            } else {
                if(outer_value != 1) {
                    os << outer_value << "sin " << "x";
                } else {
                    os << "sin " << "x";
                }
            }
            break;
        case Cos:
            if(inner_value != 1) {
                if(outer_value != 1) {
                    os << outer_value << "cos " << inner_value << "x";
                } else {
                    os << "cos " << inner_value << "x";
                }
            } else {
                if(outer_value != 1) {
                    os << outer_value << "cos " << "x";
                } else {
                    os << "cos " << "x";
                }
            }
            break;
        case Tan:
            if(inner_value != 1) {
                if(outer_value != 1) {
                    os << outer_value << "tan " << inner_value << "x";
                } else {
                    os << "tan " << inner_value << "x";
                }
            } else {
                if(outer_value != 1) {
                    os << outer_value << "tan " << "x";
                } else {
                    os << "tan " << "x";
                }
            }
            break;
        case Sec:
            if(inner_value != 1) {
                if(outer_value != 1) {
                    os << outer_value << "sec^" << 2 << " " << inner_value << "x";
                } else {
                    os << "sec^" << 2 << " " << inner_value << "x";
                }
            } else {
                if(outer_value != 1) {
                    os << outer_value << "sec^" << 2 << " x";
                } else {
                    os << "sec^" << 2 << " x";
                }
            }
            break;
    }
    return os;
}

void Node::differentiate() {
    switch (trig) {
        case None:
            outer *= exp--;
            break;
        case Sin:
            trig = Cos;
            outer *= inner;
            break;
        case Cos:
            trig = Sin;
            outer *= -1 * inner;
            break;
        case Tan:
            trig = Sec;
            outer *= inner;
            exp = 2;
            break;
        case Sec:
            break;
    }
}

bool Node::usesTrig() const {
    return trig != None;
}

bool Node::hasNext() const {
    return this->next != nullptr;
}

double Node::getOuter() const {
    return outer;
}

void Node::setOuter(double outerIn) {
    this->outer = outerIn;
}

double Node::getInner() const {
    return inner;
}

void Node::setInner(double innerIn) {
    this->inner = innerIn;
}

int Node::getNum() const {
    return num;
}

void Node::setNum(int numIn) {
    this->num = numIn;
}

int Node::getDenom() const {
    return denom;
}

void Node::setDenom(int denomIn) {
    this->denom = denomIn;
}

int Node::getExp() const {
    return exp;
}

void Node::setExp(int expIn) {
    this->exp = expIn;
}

Node* Node::getNext() const {
    return next;
}

void Node::setNext(Node* nextIn) {
    this->next = nextIn;
}

int Node::getTrig() const {
    return trig;
}

void Node::setTrig(TrigFunction trigIn) {
    trig = trigIn;
}

std::string Node::getDelimiter() const {
    return outer >= 0 ? " + " : " - ";
}
