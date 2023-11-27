#ifndef BINTREE_H
#define BINTREE_H

// Definition of a tree node
struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
};

// Definition of the binary search tree
struct BinarySearchTree {
    struct TreeNode* root;
};


//Func Proto
struct TreeNode* createNode(int ) ;
struct TreeNode* insert(struct TreeNode* , int );
void levelOrderTraversal(struct TreeNode* );
void inOrderTraversal(struct TreeNode* );
void preOrderTraversal(struct TreeNode* );
void postOrderTraversal(struct TreeNode* );
bool search(struct TreeNode* , int );

#endif