#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "BinarySearchTreeAPI.h"
#include "rules.h"

int main(int argc, char ** argv) {
    
    TreeNode* testNode;
    
    Rule* ruleOne = createRule("ruleOne", "Test One", 0, 0, 0, 0);
    Rule* ruleTwo = createRule("ruleTwo", "Test Two", 0, 0, 0, 0);
    Rule* ruleThree = createRule("ruleThree", "Test Three", 0, 0, 0, 0);
    Rule* ruleFour = createRule("ruleFour", "TestFour", 0, 0, 0, 0);
    
    
    printf("\nTest: Create a Binary Search Tree.\n");
    printf("Expected: Pointer to the tree is returned.\n");
    Tree* theTree = createBinTree(&compare, &delete);
    printf("Received: Tree address = %p\n", (void*)&theTree);
    printf("Result: %s\n\n", theTree == NULL ? "Failed" : "Passed" );
    
    printf("Test: A node is returned.\n");
    printf("Expected: A node is returned.\n");
    TreeNode* newNode = createTreeNode(ruleOne);
    printf("Received: Node with address = %p\n", (void*)&newNode);
    printf("Result: %s\n\n", newNode == NULL ? "Failed" : "Passed" );
    
    printf("Test: Add ruleTwo to the tree.\n");
    printf("Expected: ruleTwo is added to the tree.\n");
    addToTree(theTree, ruleTwo);
    testNode = findInTree(theTree, ruleTwo);
    printf("Received: Text response returned when ruleTwo is searched for = %s\n", getResponse(testNode));
    printf("Result: %s\n\n", testNode == NULL ? "Failed" : "Passed" );
    
    printf("Test: Search for ruleTwo recently added to the tree.\n");
    printf("Expected: ruleTwo is found in the tree.\n");
    testNode = findInTree(theTree, ruleTwo);
    printf("Received: Text response returned when ruleTwo is searched for = %s\n", getResponse(testNode));
    printf("Result: %s\n\n", testNode == NULL ? "Failed" : "Passed" );
    
    addToTree(theTree, ruleThree);
    addToTree(theTree, ruleFour);
    
    printf("Test: Retrieve data from the root.\n");
    printf("Expected: root data is returned.\n");
    testNode = getRootData(theTree);
    printf("Received: Text response returned when root data is searched for = %s\n", getResponse(testNode));
    printf("Result: %s\n\n", testNode == NULL ? "Failed" : "Passed" );
    
    int height = -1;
    printf("Test: Get the height of the root.\n");
    printf("Expected: Value of height is returned.\n");
    height = getHeight(theTree->root);
    printf("Received: Height of the root node = %d\n", height);
    printf("Result: %s\n\n", height == -1 ? "Failed" : "Passed" );
    
    testNode = findInTree(theTree, ruleFour);
    printf("Test: If the node containing ruleFour is a leaf.\n");
    printf("Expected: Value 1 representing true.\n");
    int leaf = isLeaf(testNode);
    printf("Received: Value = %d\n", leaf);
    printf("Result: %s\n\n", leaf == 0 ? "Failed" : "Passed" );
    
    testNode = findInTree(theTree, ruleThree);
    printf("Test: If the node containing ruleThree has two children.\n");
    printf("Expected: Value 0 representing false.\n");
    int child = hasTwoChildren(testNode);
    printf("Received: Value = %d\n", child);
    printf("Result: %s\n\n", child == 1 ? "Failed" : "Passed" );
    
    printf("Test: PrintInOrder function.\n");
    printInOrder(theTree, print);
    
    printf("Test: PrintPreOrder function.\n");
    printPreOrder(theTree, print);
    
    printf("Test: PrintPostOrder function.\n");
    printPostOrder(theTree, print);
    
    printf("\nTest: Remove ruleFour from the tree.\n");
    printf("Expected: ruleFour is removed from the tree.\n");
    removeFromTree(theTree, ruleFour);
    testNode = findInTree(theTree, ruleFour);
    printf("Received: Value returned when ruleFour is searched for = NULL\n");
    printf("Result: %s\n\n", testNode != NULL ? "Failed" : "Passed" );
    
    printf("Test: If the tree is empty.\n");
    printf("Expected: Value 0 representing false.\n");
    int empty = isTreeEmpty(theTree);
    printf("Received: Value = %d\n", empty);
    printf("Result: %s\n\n", empty != 0 ? "Failed" : "Passed" );
    
    printf("Test: Delete theTree.\n");
    printf("Expected: theTree is deleted.\n");
    destroyBinTree(theTree);
    printf("Received: %s\n", theTree ? "theTree was deleted." : "theTree was not deleted.");
    printf("Result: %s\n\n", theTree ? "Passed" : "Failed" );
    
}
