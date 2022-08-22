#define CTL_NO_TEMPLATE
#include "link.h"

#include <stdlib.h>
#include <string.h>


#define field(P, O) *((void**)(((void*)P) + O))


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
    node = field (node, next);

    for (size_t i = 0; i < *len; i++) {
        void* temp = field (node, next);
        free (node);
        node = temp;
    }

    *len = 0;
}

void cat_list (void* dest, void* srcStart, void* srcEnd, size_t next, size_t prev) {
    void* nextDest = field (dest, next);

    void* prevSrc = field (srcStart, prev);
    void* nextSrc = field (srcEnd, next);


    field (prevSrc, next) = nextSrc;
    field (nextSrc, prev) = prevSrc;

    field (srcStart, prev) = dest;
    field (srcEnd, next) = nextDest;

    field (dest, next) = srcStart;
    field (nextDest, prev) = srcEnd;
}


//
//      ============================ NODES ===============================
//

void make_node (
    void** restrict retNode, size_t nodeSize, 
    size_t next, void* restrict nextNode, size_t prev, void* restrict prevNode
) {
    *retNode = malloc (nodeSize);

    field (*retNode, next) = nextNode;
    field (*retNode, prev) = prevNode;

    field (nextNode, prev) = *retNode; 
    field (prevNode, next) = *retNode; 
}

void del_node (void* node, size_t next, size_t prev) {
    void* nextNode = field (node, next);
    void* prevNode = field (node, prev);

    field (nextNode, prev) = prevNode;
    field (prevNode, next) = nextNode;

    free (node);
}


void find_node (void** restrict node, size_t len, size_t next, size_t prev, size_t index) {
    if (index > len/2) {
        next = prev;
        index = len - index;
    }

    *node = field (*node, next);

    for (size_t i = 0; i < index; i++) {
        *node = field (*node, next);
    }
}

void swap_node (void* a, void* b, size_t next, size_t prev) {

    // Nodes linked to A and B
    void* beforeA = field (a, prev);
    void* afterA  = field (a, next);

    void* beforeB = field (b, prev);
    void* afterB  = field (b, next);

    // Swap position a B
    field (beforeA, next) = b;
    field (afterA, prev) = b;

    field (a, prev) = beforeB;
    field (a, next) = afterB;

    // Swap position of A
    field (beforeB, next) = a;
    field (afterB, prev) = a;

    field (b, prev) = beforeA;
    field (b, next) = afterA;
}

void move_node (void* targetNode, void* node, size_t next, size_t prev) {
    void* nextTarget = field (targetNode, next);

    void* beforeNode = field (node, prev);
    void* afterNode  = field (node, next);


    field (beforeNode, next) = afterNode;
    field (afterNode, prev) = beforeNode;

    field (node, next) = targetNode;
    field (node, prev) = nextTarget;

    field (targetNode, prev) = node;
    field (nextTarget, next) = node;
}

