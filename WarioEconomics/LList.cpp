//
// Created by Joseph on 10/27/2020.
//

#include "LList.h"

LList::LList() = default;

LList::LList(const LList& to_copy) {
    head = nullptr;
    Node* copy_curr = to_copy.getHead();
    while(copy_curr != nullptr) {
        *this >> new Node(*copy_curr);
        copy_curr = copy_curr->getNext();
    }
}

LList::~LList() {
    deleteNodes();
}

Node& LList::operator[](int index) const {
    Node* curr = head;
    for(int i = 0; i < index; i++) {
        curr = curr->getNext();
    }
    return *curr;
}

std::ostream& operator<<(std::ostream& os, const LList& list) {
    Node* head = list.getHead();
    if(head == nullptr) {
        return os;
    }
    std::string sign = head->getOuter() < 0 ? "-" : "";
    os << sign << *head;
    int i = 1;
    Node* curr = list.getHead()->getNext();
    while(curr != nullptr) {
        os << list[i].getDelimiter() << list[i];
        curr = curr->getNext();
        i++;
    }
    return os;
}

void LList::operator>>(Node* new_node) {
    new_node->setNext(head);
    head = new_node;
}

//finds the first occurrence of any string sequence
//returns the index of the first character in the found sequence
//so find_first_of("hel hello", "ell") would return 5
unsigned long long find_first_of(const std::string& str, std::string* seq, long long int len, int& ele) {
    ele = -1;
    unsigned long long str_len = str.length()+1;
    unsigned long long highest_index = str_len;
    for(int i = 0; i < len; i++) {
        unsigned long long index = str.find(seq[i]);
        if(index != std::string::npos && index < highest_index) {
            highest_index = index;
            ele = i;
        }
    }
    return highest_index != str_len ? highest_index : std::string::npos;
}

Node* parseTerm(const std::string& term, float multiplier) {
    Node* new_node;
    unsigned long long len = term.length();
    //parse the outer number
    long long int outer_index = term.find_first_of("xcCsStT");
    std::string outer = term.substr(0,outer_index);
    if(outer.empty()) outer = "1";
    //parse the exponent
    unsigned long long int exp_index = term.find("x^");
    std::string exp;
    if(exp_index != std::string::npos) {
        exp = term.substr(exp_index+2,len);
    } else if(term.find('x') != std::string::npos) {
        exp = "1";
    } else {
        exp = "0";
    }
    //parse for trig function and construct node
    unsigned long long trig_index;
    if((trig_index = term.find("sin")) != std::string::npos) {
        std::string inner = term.substr(trig_index+4);
        if(inner[0] == 'x') {
            new_node = new Node(stof(outer)*multiplier, 1, Sin);
        } else {
            new_node = new Node(stof(outer)*multiplier, stof(inner), Sin);
        }
    } else if((trig_index = term.find("cos")) != std::string::npos) {
        std::string inner = term.substr(trig_index+4);
        if(inner[0] == 'x') {
            new_node = new Node(stof(outer)*multiplier, 1, Cos);
        } else {
            new_node = new Node(stof(outer)*multiplier, stof(inner), Cos);
        }
    } else if((trig_index = term.find("tan")) != std::string::npos) {
        std::string inner = term.substr(trig_index+4);
        if(inner[0] == 'x') {
            new_node = new Node(stof(outer)*multiplier, 1, Tan);
        } else {
            new_node = new Node(stof(outer)*multiplier, stof(inner), Tan);
        }
    } else {
        new_node = new Node(stof(outer)*multiplier,stoi(exp));
    }
    return new_node;
}

void LList::operator>>(std::string equation) {
    std::string delimiter[2] = {" + ", " - "};
    long long int len = equation.size();
    int multiplier = 1;
    while(!equation.empty()) {
        int ele;
        unsigned long long index = find_first_of(equation, delimiter, 2, ele);
        if(index == std::string::npos) {
            std::string term = equation.substr(0,len);
            Node* new_node = parseTerm(term, static_cast<float>(multiplier));
            *this >> new_node;
            equation = "";
        } else {
            std::string term = equation.substr(0,index);
            Node* new_node = parseTerm(term, static_cast<float>(multiplier));
            *this >> new_node;
            equation = equation.substr(index+3,len);
        }
        multiplier = ele ? -1 : 1;
    }
}

void LList::deleteNode(int index) {
    if(index == 0) {
        Node* new_head = head->getNext();
        delete head;
        head = new_head;
        return;
    }
    Node* prev = head;
    for(int i = 0; i < index-1; i++) {
        prev = prev->getNext();
    }
    Node* to_delete = prev->getNext();
    prev->setNext(to_delete->getNext());
    delete to_delete;
}

void LList::purge() {
    int i = 0;
    Node* curr = head;
    while(curr != nullptr) {
        if(curr->getOuter() == 0) {
            deleteNode(i);
        }
        curr = curr->getNext();
        i++;
    }
}

void LList::differentiate() {
    Node* curr = head;
    while(curr != nullptr) {
        curr->differentiate();
        curr = curr->getNext();
    }
}

void LList::deleteNodes() {
    Node* curr = head;
    while(curr != nullptr) {
        Node* to_delete = curr;
        curr = curr->getNext();
        delete to_delete;
    }
    head = nullptr;
}

Node* getMiddle(Node* head) {
    if(head == nullptr)
        return nullptr;
    Node* fast = head;
    Node* slow = head;
    while(fast->getNext() != nullptr
          && fast->getNext()->getNext() != nullptr) {
        fast = fast->getNext()->getNext();
        slow = slow->getNext();
    }
    return slow;
}

Node* merge(Node* left, Node* right) {
    if(left == nullptr)
        return right;
    if(right == nullptr)
        return left;
    if(left->getExp() <= right->getExp()) {
        right->setNext(merge(right->getNext(), left));
        return right;
    } else {
        left->setNext(merge(left->getNext(), right));
        return left;
    }
}

void mergeSort(Node*& h) {
    if(h == nullptr || h->getNext() == nullptr)
        return;
    Node* middleNode = getMiddle(h);
    Node* middleNext = middleNode->getNext();
    middleNode->setNext(nullptr);
    mergeSort(h);
    mergeSort(middleNext);
    h = merge(h, middleNext);
}

void LList::sort() {
    if(head == nullptr) return;
    mergeSort(head);
}

Node* LList::getHead() const {
    return head;
}