#include <stdio.h>
#include <stdlib.h>
#include "linklib.h"

int main() {
    struct Node* head = NULL;

    // Adding nodes to the circular doubly linked list
    addToEnd(&head, 10);
    addToEnd(&head, 20);
    addToEnd(&head, 30);

    printf("Circular Doubly Linked List: ");
    displayList(head);

    // Adding at the beginning
    addToBeginning(&head, 5);
    printf("\nAfter adding 5 at the beginning: ");
    displayList(head);

    // Adding at an intermediate position
    addAtIntermediate(&head, 15, 2);
    printf("\nAfter adding 15 at position 2: ");
    displayList(head);

    // Deleting from the end
    deleteFromEnd(&head);
    printf("\nAfter deleting from the end: ");
    displayList(head);

    // Searching for a node
    searchNode(head, 20);

    // Deleting at an intermediate position
    deleteAtIntermediate(&head, 2);
    printf("\nAfter deleting at position 2: ");
    displayList(head);

    // Deleting from the beginning
    deleteFromBeginning(&head);
    printf("\nAfter deleting from the beginning: ");
    displayList(head);

    return 0;
}