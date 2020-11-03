#include <iostream>

using namespace std;

struct Node {
    int num = 0;
    Node* next = nullptr;
};

void enQueue(int new_num, Node* &head) {
    Node* new_head = new Node();
    new_head->num = new_num;
    if(head == nullptr) {
        head = new_head;
        return;
    }
    Node* curr = head;
    while(curr->next != nullptr) {
        curr = curr->next;
    }
    curr->next = new_head;
}

Node* deQueue(Node* &head) {
    Node* old_head = head;
    head = head->next;
    return old_head;
}

void printQueue(Node* head) {
    if(head == nullptr) {
        cout << endl;
        return;
    }
    cout << head->num << " ";
    printQueue(head->next);
}

int main()
{
    Node* head = nullptr;

    enQueue(2,head);
    enQueue(3,head);
    printQueue(head);

    deQueue(head);
    printQueue(head);
    enQueue(4,head);
    deQueue(head);
    enQueue(5,head);

    printQueue(head);

    return 0;
}
