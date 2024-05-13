///////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT (v4)
//
// By signing your name and ID below you are stating that you have agreed
// to the online academic integrity statement posted on edX:
// (Course > Assignments >  Information & Policies > Academic Integrity Policy)
///////////////////////////////////////////////////////////////////////////////
// I received help from and/or collaborated with:

// None
//
// Name: Jessie Cao
// login ID: j268cao
///////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "xml-tree.h"
#include <string.h>
#include "cs136-trace.h"
// Your additional includes go here.


// evaluate_node(n) evaluates the calculation of each node stored
// time: O(n), where n is the depth of the tree
static int evaluate_node(const struct xnode *n) {
    assert(n);
    int base = 0;
    if (strcmp(n->label, "<mul>") == 0) {
        base = 1;
        for (int i = 0; i < n->children_len; i++) {
           base *= evaluate_node(n->children[i]);
           trace_msg("inside");
        }

    } else if (strcmp(n->label, "<add>") == 0) {
        base = 0;
        for (int i = 0; i < n->children_len; i++) {
            base += evaluate_node(n->children[i]);
        }
    } else if (strcmp(n->label, "<int>") == 0) {
         return evaluate_node(n->children[0]);
        
    } else {
        return atoi(n->label);
    } 
    return base;
}

// evaluate_arithmetic_tree(tr) evaluates and outputs the calculated sum/product of the tree
// Effects: writes to the console
// time: O(n), where n is the depth of the tree
void evaluate_arithmetic_tree(struct xtree *tr) {
    assert(tr);
    printf("Evaluating %d expression(s):\n", tr->root->children_len);
    for (int i = 0; i < tr->root->children_len; i++) {    
        // Your implementation goes here.
        // * Use the following format-string to print out the result for each
        //   equation: "The result of equation #%d is %d\n"
        // * You need to implement a worker-function that traverses the xtree
        //   recursively, analyses each node (remember we made xtree a
        //   transparent structure in Q3!), and calculates the value for each
        //   node based on its type and the value of its children.

        int total = 0;
        if (strcmp(tr->root->children[i]->label, "<mul>") == 0) {
            total = evaluate_node(tr->root->children[i]);
        } else if (strcmp(tr->root->children[i]->label, "<add>") == 0) {
            total = evaluate_node(tr->root->children[i]);
        } 
        
        printf("The result of equation #%d is %d\n", i, total);

    }
}

// The calculator supports the following tags:
// * <mul>...</mul> multiplies its children together.
//   Children can be: <mul>, <add>, <int>
// * <add>...</add> adds up its children.
//   Children can be: <mul>, <add>, <int>
// * <int>...</int> an integer.
//   Children can be an integer between INT_MIN and INT_MAX.
int main(void) {
    int len = 0;
    char **tokens = tokenize_input(&len);
    struct xtree *tr = tree_create(tokens, len);

    // Render the tree as equation.
    evaluate_arithmetic_tree(tr);
    tree_destroy(tr);

    // Clean up token array.
    for (int i = 0; i < len; ++i) {
        free(tokens[i]);
    }
    free(tokens);
}
