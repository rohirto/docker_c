#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "slinklib.h"



// Function to get the current time in microseconds
long getCurrentTimeMicroseconds() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}


// Initialize the linked list
int InitLinkedList(void** list, int max_size, int is_queue) {
    if (list == NULL || *list != NULL) {
        // Invalid argument or list is already initialized
        return -1;
    }

    LinkedList* new_list = (LinkedList*)malloc(sizeof(LinkedList));
    if (new_list == NULL) {
        // Memory allocation failed
        return -2;
    }

    new_list->head = NULL;
    new_list->max_size = max_size;
    new_list->is_queue = is_queue;

    *list = (void*)new_list;
    return 0;
}

// Insert node at the specified position
int InsertNodeAt(void* list, int data, int node_position) {
    if (list == NULL) {
        // Invalid list
        return -1;
    }

    LinkedList* ll = (LinkedList*)list;

    if (node_position < 1 || node_position > ll->max_size + 1) {
        // Invalid node position
        return -2;
    }

    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        // Memory allocation failed
        return -3;
    }

    new_node->data = data;

    if (node_position == 1) {
        // Insert at the beginning
        new_node->next = ll->head;
        ll->head = new_node;
    } else {
        // Insert at the specified position
        Node* current = ll->head;
        for (int i = 1; i < node_position - 1; ++i) {
            current = current->next;
        }

        new_node->next = current->next;
        current->next = new_node;
    }

    return 0;
}

// Delete node from first or last position based on queue or stack
int DeleteNode(void* list) {
    if (list == NULL) {
        // Invalid list
        return -1;
    }

    LinkedList* ll = (LinkedList*)list;

    if (ll->head == NULL) {
        // Empty list
        return -2;
    }

    Node* temp = ll->head;

    if (ll->is_queue) {
        // Delete from the front (queue)
        ll->head = ll->head->next;
    } else {
        // Delete from the front (stack)
        ll->head = ll->head->next;
    }

    free(temp);
    return 0;
}

// Delete node from user specified node position
int DeleteNodeFrom(void* list, int node_position) {
    if (list == NULL) {
        // Invalid list
        return -1;
    }

    LinkedList* ll = (LinkedList*)list;

    if (node_position < 1 || node_position > ll->max_size) {
        // Invalid node position
        return -2;
    }

    if (ll->head == NULL) {
        // Empty list
        return -3;
    }

    Node* temp;
    if (node_position == 1) {
        // Delete from the front
        temp = ll->head;
        ll->head = ll->head->next;
    } else {
        // Delete from the specified position
        Node* current = ll->head;
        for (int i = 1; i < node_position - 1; ++i) {
            current = current->next;
        }

        temp = current->next;
        current->next = temp->next;
    }

    free(temp);
    return 0;
}

// Delete the link list
int DeleteList(void* list) {
    if (list == NULL) {
        // Invalid list
        return -1;
    }

    LinkedList* ll = (LinkedList*)list;

    Node* current = ll->head;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    ll->head = NULL;
    return 0;
}

// Reverse the linked list using a recursive method
static void RecursiveReverse(Node* current, Node* prev, Node** head) {
    if (current == NULL) {
        // Base case: end of the list
        *head = prev;
        return;
    }

    Node* next = current->next;
    current->next = prev;

    RecursiveReverse(next, current, head);
}

int ReverseList(void* list) {
    if (list == NULL) {
        // Invalid list
        return -1;
    }

    LinkedList* ll = (LinkedList*)list;

    if (ll->head == NULL) {
        // Empty list
        return -2;
    }

    RecursiveReverse(ll->head, NULL, &ll->head);

    return 0;
}

// Use recursive method to get the size of the linked list
static int RecursiveGetListSize(Node* current, int* length) {
    if (current == NULL) {
        // Base case: end of the list
        return 0;
    }

    return 1 + RecursiveGetListSize(current->next, length);
}

int GetListSize(void* list, int* length) {
    if (list == NULL || length == NULL) {
        // Invalid list or length pointer
        return -1;
    }

    LinkedList* ll = (LinkedList*)list;

    *length = RecursiveGetListSize(ll->head, length);

    return 0;
}

// Display linked list data values
void DisplayList(void* list) {
    if (list == NULL) {
        // Invalid list
        return;
    }

    LinkedList* ll = (LinkedList*)list;
    Node* current = ll->head;

    printf("Linked List: ");

    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }

    printf("\n");
}

// Bubble Sort algorithm for sorting the singly linked list
void bubbleSort(void* list) {
     if (list == NULL) {
        // Invalid list
        return;
    }

    LinkedList* ll = (LinkedList*)list;
    
    
    int swapped;
    struct Node* ptr1;
    struct Node* lptr = NULL;

    // Checking for empty list
    if (ll->head == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = ll->head;

        while (ptr1->next != lptr) {
            if (ptr1->data > ptr1->next->data) {
                // Swap the data of ptr1 and ptr1->next
                int temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Radix Sort algorithm for sorting the singly linked list
// Radix Sort algorithm for sorting the singly linked list
void radixSort(void* list) {

    if (list == NULL) {
        // Invalid list
        return;
    }

    LinkedList* ll = (LinkedList*)list;

    int max = 0;
    struct Node* temp = ll->head;

    // Find the maximum value in the list
    while (temp != NULL) {
        if (temp->data > max) {
            max = temp->data;
        }
        temp = temp->next;
    }

    // Perform counting sort for every digit
    for (int exp = 1; max / exp > 0; exp *= 10) {
        // Counting sort
        int count[10] = {0};
        struct Node* output = NULL;
        temp = ll->head;

        // Count occurrences of each digit
        while (temp != NULL) {
            count[(temp->data / exp) % 10]++;
            temp = temp->next;
        }

        // Update count[i] to store the position of the digit in the output
        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        // Build the output list
        temp = ll->head;
        while (temp != NULL) {
            struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
            newNode->data = temp->data;
            newNode->next = output;
            output = newNode;
            temp = temp->next;
        }

        // Update the original list with the sorted list
        temp = ll->head;
        struct Node* tempOutput = output;
        while (temp != NULL) {
            temp->data = tempOutput->data;
            temp = temp->next;
            tempOutput = tempOutput->next;
        }
    }
}

// Helper function to partition the linked list and return the pivot
static Node* partition(Node* head, Node* end, Node** newHead, Node** newEnd) {
    Node* pivot = end;
    Node* prev = NULL, *current = head, *tail = pivot;

    // Partition the list
    while (current != pivot) {
        if (current->data < pivot->data) {
            if ((*newHead) == NULL)
                (*newHead) = current;

            prev = current;
            current = current->next;
        } else {
            // Move current node to the end
            if (prev)
                prev->next = current->next;
            Node* temp = current->next;
            current->next = NULL;
            tail->next = current;
            tail = current;
            current = temp;
        }
    }

    // If the pivot is the smallest element
    if ((*newHead) == NULL)
        (*newHead) = pivot;

    (*newEnd) = tail;

    // Return the pivot node
    return pivot;
}

// Helper function to perform Quick Sort on the linked list
static Node* quickSortUtil(Node* head, Node* end) {
    // Base case
    if (!head || head == end)
        return head;

    Node* newHead = NULL, *newEnd = NULL;

    // Partition the list, pivot is the node at end
    Node* pivot = partition(head, end, &newHead, &newEnd);

    // If pivot is the smallest element, no need to recur for the left part
    if (newHead != pivot) {
        // Set the node before the pivot node as NULL
        Node* temp = newHead;
        while (temp->next != pivot)
            temp = temp->next;
        temp->next = NULL;

        // Recur for the list before pivot
        newHead = quickSortUtil(newHead, temp);

        // Change next of the last node of the left half to pivot
        temp = newHead;
        while (temp && temp->next)
            temp = temp->next;
        temp->next = pivot;
    }

    // Recur for the list after the pivot
    pivot->next = quickSortUtil(pivot->next, newEnd);

    return newHead;
}

// Function to sort the linked list using Quick Sort
void quickSortLinkedList(void** list) {
    if (list == NULL || (*list) == NULL)
        return;

    LinkedList* ll = (LinkedList*)(*list);

    // Get the last node of the list
    Node* end = ll->head;
    while (end->next != NULL)
        end = end->next;

    // Perform Quick Sort
    ll->head = quickSortUtil(ll->head, end);
}

// Function to sort the singly linked list using different algorithms
void sortList(void* list, int sortingMethod) {
    long startTime, endTime;
    switch (sortingMethod) {
        case BUBBLE_SORT:
            startTime = getCurrentTimeMicroseconds();
            bubbleSort(list);
            endTime = getCurrentTimeMicroseconds();
            printf("\nTime taken for Bubble Sort: %ld microseconds\n", endTime - startTime);
            break;
        case RADIX_SORT:
            startTime = getCurrentTimeMicroseconds();
            radixSort(list);
            endTime = getCurrentTimeMicroseconds();
            printf("\nTime taken for Radix Sort: %ld microseconds\n", endTime - startTime);
            break;
        case QUICK_SORT:
            startTime = getCurrentTimeMicroseconds();
            quickSortLinkedList(&list);
            endTime = getCurrentTimeMicroseconds();
            printf("\nTime taken for Quick Sort: %ld microseconds\n", endTime - startTime);
            break;
        default:
            printf("\nInvalid sorting method\n");
    }
}