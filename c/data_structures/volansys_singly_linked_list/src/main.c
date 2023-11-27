
#include <stdio.h>
#include <stdlib.h>
#include "slinklib.h"

// Example usage
int main() {
    void* list = NULL;  // Make sure the pointer is initialized to NULL;
    int length;

    // Initialize linked list as a queue with a maximum size of 10
    int result = InitLinkedList(&list, 10, 1);
    if (result != 0) {
        printf("Initialization failed with error code: %d\n", result);
        return 1;
    }

    // Insert nodes at different positions
    InsertNodeAt(list, 1, 1);
    InsertNodeAt(list, 2, 2);
    InsertNodeAt(list, 3, 1);
    DisplayList(list);

    // Delete a node from the front
    DeleteNode(list);
    DisplayList(list);

    // Reverse the list
    ReverseList(list);
    DisplayList(list);

    // Get the size of the list
    GetListSize(list, &length);
    printf("List size: %d\n", length);

    //Sort FFunctions
    // Adding nodes to the singly linked list
    InsertNodeAt(list, 40,1);
    InsertNodeAt(list, 20,1);
    InsertNodeAt(list, 30,1);
    InsertNodeAt(list, 10,1);

    printf("Original Singly Linked List: ");
    DisplayList(list);

     // Sorting the list using Bubble Sort
    sortList(list, BUBBLE_SORT);
    printf("\nAfter Bubble Sort: ");
    DisplayList(list);

    // Sorting the list using Radix Sort
    sortList(list, RADIX_SORT);
    printf("\nAfter Radix Sort: ");
    DisplayList(list);

    

    sortList(list, QUICK_SORT);
    printf("\nAfter Quick Sort: ");
    DisplayList(list);

    // Delete the list
    DeleteList(list);

    return 0;
}

