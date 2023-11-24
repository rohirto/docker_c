#include <stdio.h>
#include <stdlib.h>
#include "linklib.h"


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

