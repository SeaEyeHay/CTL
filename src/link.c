#define CTL_NO_TEMPLATE
#include "link.h"

#include "macros.h"

#include <stdlib.h>
#include <string.h>


//
//      ============================ LIST ===============================
//

void make_list (size_t item, size_t* len, void* dummy, void* this, void** next, void** prev) {
    *len = 0;

    memset (this, 0, item);

    *next = dummy;
    *prev = dummy;
}

void del_list (void* restrict node, size_t next, size_t* restrict len) {
    node = field (node, next, void*);

    for (size_t i = 0; i < *len; i++) {
        void* temp = field (node, next, void*);
        free (node);
        node = temp;
    }

    *len = 0;
}

void cat_list (void* dest, void* srcStart, void* srcEnd, size_t next, size_t prev) {
    void* nextDest = field (dest, next, void*);

    void* prevSrc = field (srcStart, prev, void*);
    void* nextSrc = field (srcEnd, next, void*);


    field (prevSrc, next, void*) = nextSrc;
    field (nextSrc, prev, void*) = prevSrc;

    field (srcStart, prev, void*) = dest;
    field (srcEnd, next, void*) = nextDest;

    field (dest, next, void*) = srcStart;
    field (nextDest, prev, void*) = srcEnd;
}


//
//      ============================ NODES ===============================
//

void make_node (
    void** restrict retNode, size_t nodeSize, 
    size_t next, void* restrict nextNode, size_t prev, void* restrict prevNode
) {
    *retNode = malloc (nodeSize);

    field (*retNode, next, void*) = nextNode;
    field (*retNode, prev, void*) = prevNode;

    field (nextNode, prev, void*) = *retNode; 
    field (prevNode, next, void*) = *retNode; 
}

void del_node (void* node, size_t next, size_t prev) {
    void* nextNode = field (node, next, void*);
    void* prevNode = field (node, prev, void*);

    field (nextNode, prev, void*) = prevNode;
    field (prevNode, next, void*) = nextNode;

    free (node);
}


void find_node (void** restrict node, size_t len, size_t next, size_t prev, size_t index) {
    if (index > len/2) {
        next = prev;
        index = len - index;
    }

    *node = field (*node, next, void*);

    for (size_t i = 0; i < index; i++) {
        *node = field (*node, next, void*);
    }
}

void swap_node (void* a, void* b, size_t next, size_t prev) {

    // Nodes linked to A and B
    void* beforeA = field (a, prev, void*);
    void* afterA  = field (a, next, void*);

    void* beforeB = field (b, prev, void*);
    void* afterB  = field (b, next, void*);

    // Swap position a B
    field (beforeA, next, void*) = b;
    field (afterA, prev, void*) = b;

    field (a, prev, void*) = beforeB;
    field (a, next, void*) = afterB;

    // Swap position of A
    field (beforeB, next, void*) = a;
    field (afterB, prev, void*) = a;

    field (b, prev, void*) = beforeA;
    field (b, next, void*) = afterA;
}

void move_node (void* targetNode, void* node, size_t next, size_t prev) {
    void* nextTarget = field (targetNode, next, void*);

    void* beforeNode = field (node, prev, void*);
    void* afterNode  = field (node, next, void*);


    field (beforeNode, next, void*) = afterNode;
    field (afterNode, prev, void*) = beforeNode;

    field (node, next, void*) = targetNode;
    field (node, prev, void*) = nextTarget;

    field (targetNode, prev, void*) = node;
    field (nextTarget, next, void*) = node;
}

