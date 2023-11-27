#ifndef SLINKLIB_H
#define SLINKLIB_H


// Sorting method constants
#define BUBBLE_SORT 1
#define RADIX_SORT 2
#define QUICK_SORT 3

// Structure for a node
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Structure for the linked list
typedef struct LinkedList {
    Node* head;
    int max_size;
    int is_queue; // Flag to indicate if the linked list acts as a queue (1) or stack (0)
} LinkedList;


int InitLinkedList(void** , int , int );
int InsertNodeAt(void* , int , int );
int DeleteNode(void* );
int DeleteNodeFrom(void* , int );
int DeleteList(void* );
static void RecursiveReverse(Node* , Node* , Node** );
int ReverseList(void* ) ;
static int RecursiveGetListSize(Node* , int* );
int GetListSize(void* list, int* );
void DisplayList(void* );
void sortList(void* , int );




#endif