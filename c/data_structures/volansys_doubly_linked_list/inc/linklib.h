#ifndef LINKLIB_H
#define LINKLIB_H

// Node structure
struct Node {
    int data;
    struct Node* next;
    struct Node* prev;
};

// Function prototypes
void addToBeginning(struct Node** head, int data);
void addToEnd(struct Node** head, int data);
void addAtIntermediate(struct Node** head, int data, int position);
void deleteFromBeginning(struct Node** head);
void deleteFromEnd(struct Node** head);
void deleteAtIntermediate(struct Node** head, int position);
void searchNode(struct Node* head, int key);
void displayList(struct Node* head);

#endif