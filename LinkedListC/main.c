#include <stdio.h>
#include <stdlib.h>

struct Node {
    int num;
    struct Node* next;
};

void addList(int num, struct Node** head) {
    struct Node* new_node = (struct Node*)(malloc(sizeof(struct Node)));
    new_node->num = num;
    new_node->next = NULL;
    struct Node* curr = *head;
    if(*head == NULL) {
        *head = new_node;
        return;
    }
    while(curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;
}

void printList(struct Node* head) {
    struct Node* curr = head;
    while(curr != NULL) {
        printf("%d",curr->num);
        curr = curr->next;
    }
    printf("\n");
}

void insList(int pos, struct Node* head, struct Node* to_add) {
    struct Node* ins_pos = head;
    for(int i= 0; i < pos; i++) {
        ins_pos = ins_pos->next;
    }
    struct Node* next = ins_pos->next;
    ins_pos->next = to_add;
    to_add->next = next;
}

void deleteFromList(int pos, struct Node* head) {
    struct Node* prev = head;
    for(int i = 0; i < pos-1; i++) {
        prev = prev->next;
    }
    struct Node* to_delete = prev->next;
    struct Node* next = to_delete->next;
    prev->next = next;
    free(to_delete);
}

void deleteList(struct Node* head) {
    struct Node* curr = head;
    while(curr != NULL) {
        struct Node* to_delete = curr;
        curr = curr->next;
        free(to_delete);
    }
}

void moveElement(struct Node* head, int from, int to) {
    struct Node* prev = head;
    for(int i = 0; i < from-1; i++) {
        prev = prev->next;
    }
    struct Node* to_move = prev->next;
    struct Node* next = to_move->next;
    prev->next = next;
    prev = head;
    for(int i = 0; i < to-1; i++) {
        prev = prev->next;
    }
    next = prev->next;
    prev->next = to_move;
    to_move->next = next;
}

int main() {

    struct Node* head = NULL;

    addList(0,&head);
    addList(1,&head);
    addList(2,&head);
    addList(3,&head);
    printList(head);

    struct Node* new_node = (struct Node*)(malloc(sizeof(struct Node)));
    new_node->num = 4;
    new_node->next = NULL;
    insList(0,head,new_node);
    printList(head);

    moveElement(head,1,3);
    printList(head);

    deleteFromList(1,head);
    printList(head);

    deleteList(head);

    return 0;
}
