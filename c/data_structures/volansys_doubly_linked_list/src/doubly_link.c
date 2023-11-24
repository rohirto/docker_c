#include <stdio.h>
#include <stdlib.h>
#include "linklib.h"

// Function to add a node to the beginning of the circular doubly linked list
void addToBeginning(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    newNode->prev = (*head)->prev;
    (*head)->prev->next = newNode;
    (*head)->prev = newNode;
    *head = newNode;
}

// Function to add a node to the end of the circular doubly linked list
void addToEnd(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    newNode->prev = (*head)->prev;
    (*head)->prev->next = newNode;
    (*head)->prev = newNode;
}

// Function to add a node at an intermediate position in the circular doubly linked list
void addAtIntermediate(struct Node** head, int data, int position) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    struct Node* temp = *head;

    for (int i = 1; i < position - 1; i++) {
        temp = temp->next;
    }

    newNode->next = temp->next;
    newNode->prev = temp;
    temp->next->prev = newNode;
    temp->next = newNode;
}

// Function to delete a node from the beginning of the circular doubly linked list
void deleteFromBeginning(struct Node** head) {
    struct Node* temp = *head;

    if (*head == NULL) {
        return;
    }

    if ((*head)->next == *head) {
        *head = NULL;
    } else {
        (*head)->prev->next = (*head)->next;
        (*head)->next->prev = (*head)->prev;
        *head = (*head)->next;
    }

    free(temp);
}

// Function to delete a node from the end of the circular doubly linked list
void deleteFromEnd(struct Node** head) {
    struct Node* temp = *head;

    if (*head == NULL) {
        return;
    }

    if ((*head)->next == *head) {
        *head = NULL;
    } else {
        (*head)->prev->prev->next = *head;
        (*head)->prev = (*head)->prev->prev;
    }

    free(temp->prev);
}

// Function to delete a node at an intermediate position in the circular doubly linked list
void deleteAtIntermediate(struct Node** head, int position) {
    struct Node* temp = *head;

    for (int i = 1; i < position; i++) {
        temp = temp->next;
    }

    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    free(temp);
}

// Function to search for a node with a given key in the circular doubly linked list
void searchNode(struct Node* head, int key) {
    struct Node* temp = head;
    int position = 1;

    while (temp->data != key && temp->next != head) {
        temp = temp->next;
        position++;
    }

    if (temp->data == key) {
        printf("\nNode with data %d found at position %d\n", key, position);
    } else {
        printf("\nNode with data %d not found in the list\n", key);
    }
}

// Function to display the circular doubly linked list
void displayList(struct Node* head) {
    struct Node* temp = head;

    if (head == NULL) {
        printf("Empty list");
        return;
    }

    do {
        printf("%d ", temp->data);
        temp = temp->next;
    } while (temp != head);

    printf("\n");
}