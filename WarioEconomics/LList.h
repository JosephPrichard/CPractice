//
// Created by Joseph on 10/27/2020.
//

#ifndef WARIOECONOMICS_LLIST_H
#define WARIOECONOMICS_LLIST_H

#include <iostream>
#include "Node.h"

class LList {

private:
    Node* head = nullptr;

public:
    LList();
    LList(const LList& to_copy);
    ~LList();

    Node& operator[](int index) const;
    friend std::ostream& operator<<(std::ostream& os, const LList& list);
    void operator>>(Node* new_node);
    void operator>>(std::string string);

    void differentiate();
    void deleteNodes();
    void sort();
    void purge();
    void deleteNode(int index);

    Node* getHead() const;

};

#endif //WARIOECONOMICS_LLIST_H