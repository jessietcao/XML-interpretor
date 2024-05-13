#ifndef __XMLTREE
#define __XMLTREE

// An XML-tree.
struct xtree {
    struct xnode *root; // root of the XML-tree
};

// A node in an XML-tree.
struct xnode {
    const char *label;       // token stored in this node
    struct xnode **children; // array of children
    int children_len;        // length of children array, TEXT_NODE if leaf
};

#endif // __XMLTREE

// Indicates that an XML-node is a text node, i.e., a leaf.
extern const int TEXT_NODE;

// The label for the root node of the XML tree.
extern const char *ROOT_TAG;

// tree_create(tokens, len) creates an XML-tree that contains all strings from
//   the array tokens of length len.
// effects: allocates memory [client must call tree_destroy]
struct xtree *tree_create(char **tokens, int len);

// tree_destroy(tr) releases all resources used by XML tree *tr.
// effects: invalidates *tr
void tree_destroy(struct xtree *tr);

// tree_print(tr) prints out the content of XML tree *tr.
// effects: writes to output
void tree_print(struct xtree *tr);
