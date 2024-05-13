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

#include "xml-tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "collector.h"
#include "cs136-trace.h"
// Your additional includes go here.

// Indicates that an XML-node is a text node, i.e., a leaf.
const int TEXT_NODE = -1;

// The label for the root node of the XML tree.
const char *ROOT_TAG =  "<%%root%%>";

// === XML-Tree Nodes =========================================================

// A node in an XML-tree.
struct xnode {
    const char *label;       // token stored in this node
    struct xnode **children; // array of children
    int children_len;        // length of children array, TEXT_NODE if leaf
};

// node_print(v) prints out the xnode *v.
// effects: writes to output
//// time: O(1)
void node_print(const void *v) {
    assert(v);
    const struct xnode *xn = v;
    printf("[%s: %d]\n", xn->label, xn->children_len);
}

// node_make(lbl, tokens, len) creates a subtree from *tokens with lbl as the
//   label for the root node. The labels for nodes in the subtree are generated
//   by processing strings in *tokens, an array of length *len. The function
//   advances *tokens and decrements *len by the number of tokens processed.
// effects: allocates memory [client must call node_destroy]
//          mutates **tokens
//          mutates *len
// time: O(n^2 * o), where n is the length of tokens and o is the size of xnode
struct xnode *node_make(const char *label, char ***tokens, int *len) {
    // Your implementation goes here.
    //tokens is a pointer to list of tokens
    //*len pointer to length of array

    //pseudo
    //assign a xnode
    //label is xnode's name
    //if label is markup:
    //---> if start markup
    //          ->go one level down
    //---> if end markup
    //          ->retrieve
    //If label is content:
    //---> set children none and len is -1
    //
    //set xnode
    //malloc collector children list of xnodes
    //set collector insert the recursed children nodes
    //collpase the collector and set to array
    //assign array as children's xnodes

    assert(label);
    assert(tokens);
    struct xnode *curr_node = malloc(sizeof (struct xnode)); 
    assert(curr_node);

    if (*label == '<') { //start markup 
        int count_children = 0;
        curr_node->label = label;
        struct collector *children = collector_init(sizeof(struct xnode *), node_print);

        while (*len != 0){
            char *new_label = (*tokens)[0];
            (*len)--;
            if (*new_label == '<' && (new_label + 1) && *(new_label + 1) == '/') {
                ++(*tokens);
                   break;
            } else{
                ++(*tokens);
                struct xnode *ntemp = node_make(new_label, tokens, len);
                collector_insert(children, &ntemp);
            } 
        }

        struct xnode **children_arr = collector_collapse(children, &count_children);
        curr_node->children_len = count_children; 
        curr_node->children = children_arr;
    } else { //content
        curr_node->label = label;
        curr_node->children = NULL;
        curr_node->children_len = TEXT_NODE;
    }

    return curr_node;
}



// node_destroy(xn) releases all resources use by XML-node *xn, including its
//   children.
// effects: invalidates *xn
// time: O(n^2)
void node_destroy(struct xnode *xn) {  
    // Your implementation goes here.
    assert(xn);
    for (int i = 0; i < xn->children_len; i++) {
        node_destroy(xn->children[i]);
    }
    if (xn->children){
        free(xn->children);
    }
    free(xn);

}

// === XML-Tree ===============================================================

// See documentation in tree.h.
struct xtree {
    struct xnode *root; // root of the XML-tree
};

// See documentation in tree.h.
struct xtree *tree_create(char **tokens, int len) {
    assert(tokens);
    struct xtree *tr = malloc(sizeof(struct xtree));  
    assert(tr);
    if (len == 0) {
        tr->root = NULL;
    } else {
        tr->root = node_make(ROOT_TAG, &tokens, &len);
    }
    return tr;
}

// See documentation in tree.h.
void tree_destroy(struct xtree *tr) {
    assert(tr);
    if (tr->root != NULL) {
        node_destroy(tr->root);
    }
    free(tr);
}

// tree_print_height(tr, height) prints out the content of XML tree *tr accoring to its hierarchy
// effects: writes to output
void tree_print_height(struct xtree *tr, int height) {
    assert(tr);
    // Your call to your (recursive) worker function goes here.
    for (int j = 0; j < height; j++) {
            printf("  "); 
    }

    printf("%s",tr->root->label);
    printf("\n");

    struct xtree *new_tree = malloc(sizeof(struct xtree));
    assert(new_tree);
    for (int i = 0; i < tr->root->children_len; i++) {
        new_tree->root = (tr->root->children)[i];
        tree_print_height(new_tree, height+1);
    }

    if (*tr->root->label == '<'){
        for (int j = 0; j < height; j++){
            printf("  ");
        }

        printf("</");
        printf("%s\n",tr->root->label+=1);
    }

    free(new_tree);
}


// See documentation in tree.h.
void tree_print(struct xtree *tr) {
    assert(tr);
    
    if (tr->root != NULL) {
        tree_print_height(tr, 0);
    } else {
        printf("%s\n</%s\n", ROOT_TAG, ROOT_TAG + 1);
    }    
}
