/**
 * @file main.c
 * @author rohirto
 * @brief Binary Tree algo
 * @version 0.1
 * @date 2023-11-27
 * 
 * @copyright Copyright (c) 2023
 * 
 * Develop a program to construct a binary search tree.
 * Tree data will be given by user, user enters EOF to express end of list.
 * Display level-order traversal to validate the sequence of integers entered by user.
 * Display BST tree in-order, pre-order and post-order traversal format.
 * Display timing and memory space information for each type of traversal
 * Input the same list in program 1 and perform search operation. Compared timing and space information to understand difference between different types of search.
 *
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "bintree.h"





// Measure time and memory for traversal
void measureTimeAndMemory(void (*traversalFunction)(struct TreeNode*), struct TreeNode* root, const char* traversalType) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    traversalFunction(root);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("\n%s Traversal Time: %f seconds\n", traversalType, cpu_time_used);
}

int main() {
    struct BinarySearchTree bst;
    bst.root = NULL;

    printf("Enter integers to construct the binary search tree (enter -1 to finish):\n");
    int value;
    while (true) {
        scanf("%d", &value);
        if (value == -1) {
            break;
        }
        bst.root = insert(bst.root, value);
    }

    // Validate level-order traversal
    printf("Level-order traversal: ");
    levelOrderTraversal(bst.root);
    printf("\n");

    // Display tree traversals
    printf("In-order traversal: ");
    inOrderTraversal(bst.root);
    printf("\n");

    printf("Pre-order traversal: ");
    preOrderTraversal(bst.root);
    printf("\n");

    printf("Post-order traversal: ");
    postOrderTraversal(bst.root);
    printf("\n");

    // Measure time for each traversal
    measureTimeAndMemory(inOrderTraversal, bst.root, "In-order");
    measureTimeAndMemory(preOrderTraversal, bst.root, "Pre-order");
    measureTimeAndMemory(postOrderTraversal, bst.root, "Post-order");

    // Perform search operation and compare time
    printf("\nEnter a value to search in the binary search tree:\n");
    int searchKey;
    scanf("%d", &searchKey);
    clock_t searchStart = clock();
    bool found = search(bst.root, searchKey);
    clock_t searchEnd = clock();
    double searchTime = ((double)(searchEnd - searchStart)) / CLOCKS_PER_SEC;
    printf("Time taken for search: %f seconds\n", searchTime);

    return 0;
}
