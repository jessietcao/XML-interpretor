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

#include "collector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cs136-trace.h"
// Your additional includes go here.

// === Collector Nodes ========================================================

// A node in a collector
struct col_node {
    void *data;            // The stored data
    struct col_node *next; // The next node
};

// cnode_create(obj, obj_size) creates a new collector node that stores a copy
//   of the object *obj of size obj_size.
// requires: *obj is of size obj_size [not asserted]
// effects:  allocates heap memory [client must call cnode_destroy]
// time:     O(o), where o is the object size
struct col_node *cnode_create(const void *obj, size_t obj_size) {
    assert(obj);
    struct col_node *cn = malloc(sizeof(struct col_node));
    assert(cn);
    cn->data = malloc(obj_size);
    assert(cn->data);
    // void *memcpy(void *dest, const void *src, size_t count) copies count
    //   bytes from *src to *dest. Here it is used to copy one object of size
    //   obj_size from *obj into *cn->data. Since memcpy blindly copies a set
    //   amount of bytes, it does not need to know the underlying structure
    //   stored at the source (i.e., *obj).
    memcpy(cn->data, obj, obj_size);
    cn->next = NULL;
    return cn;
}

// cnode_destroy(cn) releases all resources used by *cn directly.
// effects:  invalidates *cn
// time:     O(1)
void cnode_destroy(struct col_node *cn) {
    assert(cn);
    free(cn->data);
    free(cn);
}

// === Collector ==============================================================

// See documentation in collector.h.
struct collector {
    struct col_node *front;          // front node
    int obj_size;                    // size of a single object in bytes
    void (*obj_print)(const void *); // function to print a single object
    // Your additional fields go here.
};

// See documentation in collector.h.
struct collector *collector_init(int obj_size, void (*obj_print)(const void *)) {
    // Your implementation goes here.
    assert(obj_print);
    struct collector * c = malloc(sizeof(struct collector));
    assert(c);
    c->front = NULL;
    c->obj_size = obj_size;
    c->obj_print = obj_print;

    return c;
}

// See documentation in collector.h.
void collector_insert(struct collector *col, const void *obj) {
    // Your implementation goes here.
    assert(col);
    assert(obj);

    void *saved_obj = malloc(col->obj_size);
    memcpy(saved_obj, obj, col->obj_size);

    if (!col->front){  
        col->front = cnode_create(saved_obj, col->obj_size);
    } else {
        struct col_node *temp = col->front;
        while (temp->next) {
            temp = temp->next;
        }

        temp->next = cnode_create(saved_obj, col->obj_size);
    }
    free(saved_obj);
}

// See documentation in collector.h.
void *collector_collapse(struct collector *col, int *len) {
    // Your implementation goes here.
    assert(col);
    assert(len);
    int count = 0;
    
    struct col_node *ptr = col->front;
    while (ptr) {
        count++;
        ptr = ptr->next;
    }

    *len = count;

    void *arr = malloc(col->obj_size * count);
    void *p = arr;

    while (col->front) {
        struct col_node *temp = col->front->next;
        memcpy(p, col->front->data, col->obj_size);
        p += col->obj_size;
        cnode_destroy(col->front);
        col->front = temp;
    }

    free(col);

    return arr;
}

// See documentation in collector.h.
void collector_print(const struct collector *col) {
    // Your implementation goes here.
    // The print format is: "[OBJECT, OBJECT, OBJECT, ...]\n"
    //   where OBJECT is the printout of an object by col->obj_print,
    //   adjacent OBJECTs are separated by ", ".
    //   If the collector is empty, the function should print: "[]\n"

    assert(col);

    struct col_node *ptr = col->front;
    printf("[");

    while (ptr && ptr->next) {
        col->obj_print(ptr->data);
        printf(", ");
        ptr = ptr->next;
    }

    if (ptr){
        col->obj_print(ptr->data);
    }

    printf("]\n");
    
}
