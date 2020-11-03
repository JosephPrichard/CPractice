#include <stdio.h>
#include<stdlib.h>
#include <stdbool.h>

struct Node {
    int num;
    struct Node* next;
};

void enQueue(int num, struct Node** head) {
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

void printQueue(struct Node* head) {
    if(head == NULL) {
        printf("\n");
        return;
    }
    printf("%d", head->num);
    printf(" ");
    printQueue(head->next);
}

void bubbleSort(struct Node* head) {
    bool notSorted = true;
    while(notSorted) {
        notSorted = false;
        struct Node* curr = head;
        while(curr->next != NULL) {
            if(curr->num > curr->next->num) {
                int temp = curr->next->num;
                curr->next->num = curr->num;
                curr->num = temp;
                notSorted = true;
            }
            curr = curr->next;
        }
        printQueue(head);
    }
}

void selectionSort(struct Node* head) {
    struct Node* curr = head;
    while(curr->next != NULL) {
        struct Node* best = curr;
        struct Node* curr_best = best->next;
        while(curr_best != NULL) {
            if(curr_best->num < best->num) {
                best = curr_best;
            }
            curr_best = curr_best->next;
        }
        int temp = best->num;
        best->num = curr->num;
        curr->num = temp;
        curr = curr->next;
        printQueue(head);
    }
}

int main() {

    struct Node* head_one = NULL;
    struct Node* head_two = NULL;

    for(int i = 0; i < 8; i++) {
        int val;
        printf("Enter a number: ");
        scanf("%i", &val);
        enQueue(val, &head_one);
        enQueue(val, &head_two);
    }

    //printf("Bubble sort: ");
    printf("\n");

    printQueue(head_one);
    bubbleSort(head_one);

    //printf("SelectionSort: ");
    printf("\n");

    printQueue(head_two);
    selectionSort(head_two);

    return 0;
}