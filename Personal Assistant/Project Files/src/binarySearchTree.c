
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BinarySearchTreeAPI.h"
#include "rules.h"

TreeNode* createTreeNode(TreeDataPtr data) {
    
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    
    return newNode;
}


Tree* createBinTree(CompareFunc compare, DeleteFunc del) {
    
    Tree* newTree = (Tree*)malloc(sizeof(Tree));
    
    newTree->root = NULL;
    
    newTree->compareFunc = compare;
    newTree->deleteFunc = del;
    
    return newTree;
}


void destroyBinTree(Tree * toDestroy) {
    
    destroyTree(toDestroy->root);
}

void destroyTree(TreeNode* node) {
    
    if(node == NULL)
        return;
    
    destroyTree(node->left);
    destroyTree(node->right);
    
    free(node);
}


void addToTree(Tree * theTree, TreeDataPtr data) {
    
    theTree->root = insert(theTree->root, data, theTree->compareFunc);
}


TreeNode* insert(TreeNode* currentNode, TreeDataPtr data, CompareFunc compareFunc) {
    
    if(currentNode == NULL)
        return createTreeNode(data);
    
    if(compareFunc(currentNode->data, data) > 0) {
        
        currentNode->right = insert(currentNode->right, data, compareFunc);
        
    }else if(compareFunc(currentNode->data, data) < 0) {
        
        currentNode->left = insert(currentNode->left, data, compareFunc);
    }
    
    return currentNode;
}


void removeFromTree(Tree * theTree, TreeDataPtr data) {
    
    deleteNode(theTree->root, data, theTree->compareFunc);
}

TreeNode* deleteNode(TreeNode* currentNode, TreeDataPtr data, CompareFunc compareFunc) {
    
    if(currentNode == NULL)
        return currentNode;
    
    if(compareFunc(currentNode->data, data) > 0) {
        
        currentNode->right = deleteNode(currentNode->right, data, compareFunc);
        
    }else if(compareFunc(currentNode->data, data) < 0) {
        
        currentNode->left = deleteNode(currentNode->left, data, compareFunc);
    
    }else {
        
        if(currentNode->left == NULL) {
            
            TreeNode* temp = currentNode->right;
            free(currentNode);
            return temp;
            
        }else if(currentNode->right == NULL) {
            
            TreeNode* temp = currentNode->left;
            free(currentNode);
            return temp;
        }
        
        TreeNode* temp = getMinVal(currentNode->right);
        
        currentNode->data = temp->data;
        
        currentNode->right = deleteNode(currentNode->right, temp->data, compareFunc);
    }
    
    return currentNode;
}

TreeNode* getMinVal(TreeNode* node) {
    
    while(node->left != NULL)
        node = node->left;
        
    return node;
}


TreeDataPtr findInTree(Tree* theTree, TreeDataPtr data ) {
    
    return find(theTree->root, data, theTree->compareFunc);
}

TreeNode* find(TreeNode* currentNode, TreeDataPtr data, CompareFunc compareFunc) {
    
    if(currentNode == NULL)
        return NULL;
    
    if(compareFunc(currentNode->data, data) == 0) {
        
        return currentNode->data;
    
    }else if(compareFunc(currentNode->data, data) > 0) {
       
        return find(currentNode->right, data, compareFunc);
    
    }else if(compareFunc(currentNode->data, data) < 0) {
        
        return find(currentNode->left, data, compareFunc);
    }
    
    return NULL;
}


TreeDataPtr getRootData(Tree * theTree) {
    
    if(theTree->root == NULL)
        return NULL;
    
    return theTree->root->data;
}


void printInOrder(Tree * theTree, PrintFunc printData) {
    
    inOrder(theTree->root, print);
}


void inOrder(TreeNode* node, PrintFunc printData) {
    
    if(node != NULL) {
        
        inOrder(node->left, printData);
        printData(node->data);
        inOrder(node->right, printData);
    }
}


void printPreOrder(Tree * theTree, PrintFunc printData) {
    
    preOrder(theTree->root, print);
}

void preOrder(TreeNode* node, PrintFunc printData) {
    
    if(node == NULL)
        return;
    
    printData(node->data);
    
    preOrder(node->left, printData);
    preOrder(node->right, printData);
}

void printPostOrder(Tree * theTree, PrintFunc printData) {
    
    postOrder(theTree->root, print);
}

void postOrder(TreeNode* node, PrintFunc printData) {
    
    if(node == NULL)
        return;
    
    postOrder(node->left, printData);
    postOrder(node->right, printData);
    
    printData(node->data);
}


int isTreeEmpty(Tree* theTree) {
    
    if(theTree->root == NULL)
        return 1;
    
    return 0;
}


int isLeaf(TreeNode * treeNode) {
    
    if(treeNode == NULL)
        return 0;
    
    if(treeNode->left == NULL && treeNode->right == NULL)
        return 1;
        
    return 0;
}


int hasTwoChildren(TreeNode *treeNode) {
    
    if(treeNode == NULL)
        return 0;
    
    if(treeNode->left != NULL && treeNode->right != NULL)
        return 1;
    
    return 0;
}


int getHeight(TreeNode* treeNode) {
    
    int out = 0;
    
    if(treeNode == NULL)
        return 0;
    
    int left = getHeight(treeNode->left);
    int right = getHeight(treeNode->right);
    
    if(left > right) {
        
        out = left + 1;
        
    }else {
        
        out = right + 1;
    }
    
    return out;
}


