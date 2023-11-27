/**
 * @file bintree.c
 * @author rohirto
 * @brief Functions APIs for binary tree
 * @version 0.1
 * @date 2023-11-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "bintree.h"

// Function to create a new tree node
struct TreeNode* createNode(int key) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->val = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a key into the BST
struct TreeNode* insert(struct TreeNode* root, int key) {
    if (root == NULL) {
        return createNode(key);
    }

    if (key < root->val) {
        root->left = insert(root->left, key);
    } else if (key > root->val) {
        root->right = insert(root->right, key);
    }

    return root;
}

// Function for level-order traversal using a queue
void levelOrderTraversal(struct TreeNode* root) {
    if (root == NULL) {
        return;
    }

    struct TreeNode* queue[1000];  // Assuming a maximum of 1000 nodes
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        struct TreeNode* current = queue[front++];
        printf("%d ", current->val);

        if (current->left != NULL) {
            queue[rear++] = current->left;
        }
        if (current->right != NULL) {
            queue[rear++] = current->right;
        }
    }
}

// Function for in-order traversal
void inOrderTraversal(struct TreeNode* root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%d ", root->val);
        inOrderTraversal(root->right);
    }
}

// Function for pre-order traversal
void preOrderTraversal(struct TreeNode* root) {
    if (root != NULL) {
        printf("%d ", root->val);
        preOrderTraversal(root->left);
        preOrderTraversal(root->right);
    }
}

// Function for post-order traversal
void postOrderTraversal(struct TreeNode* root) {
    if (root != NULL) {
        postOrderTraversal(root->left);
        postOrderTraversal(root->right);
        printf("%d ", root->val);
    }
}

// Function to search for a key in the BST
bool search(struct TreeNode* root, int key) {
    while (root != NULL) {
        if (key < root->val) {
            root = root->left;
        } else if (key > root->val) {
            root = root->right;
        } else {
            return true;  // Key found
        }
    }
    return false;  // Key not found
}

