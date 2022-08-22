#ifndef CTL_LINK_H
#define CTL_LINK_H

#include <stddef.h>


extern void make_list (size_t item, size_t* len, void* dummy, void* this, void** next, void** prev);

extern void del_list (void* restrict node, size_t next, size_t* restrict len);


extern void make_node (void** restrict retNode, size_t nodeSize, 
                       size_t next, void* restrict nextNode, size_t prev, void* restrict prevNode);

extern void del_node (void* node, size_t next, size_t prev);


extern void find_node (void** restrict node, size_t len, size_t next, size_t prev, size_t index);


#endif // CTL_LINK_H


//
//      ================================= TEMPLATE =================================
//

#ifndef CTL_NO_TEMPLATE

#define CTL_STRUCT_SFX l
#define CTL_NODE_SFX lnk
#include "template.h"


#ifndef CTL_IMPLEMENTATION

struct CTL_NODE {
    CTL_TYPE_ID this;

    struct CTL_NODE* next;
    struct CTL_NODE* prev;
};

struct CTL_STRUCT {
    size_t len;
    struct CTL_NODE links;
};

#endif // CTL_IMPLEMENTATION


CTL_INLINE CTL_TYPE_ID DEF_METHODE(ln, get, CTL_TYPE_NAME) (struct CTL_STRUCT* ln, size_t i) fn (
    struct CTL_NODE* node = &ln->links;
    find_node ((void**) &node, ln->len, offsetof(struct CTL_NODE, next), offsetof(struct CTL_NODE, prev), i);

    return node->this;
)

CTL_INLINE struct CTL_NODE* DEF_METHODE(ln, find, CTL_TYPE_NAME) (struct CTL_STRUCT* ln, size_t i) fn (
    struct CTL_NODE* node = &ln->links;
    find_node ((void**) &node, ln->len, offsetof(struct CTL_NODE, next), offsetof(struct CTL_NODE, prev), i);

    return node;
) 

CTL_INLINE CTL_TYPE_ID DEF_METHODE(ln, set, CTL_TYPE_NAME) (struct CTL_STRUCT* ln, size_t i, CTL_TYPE_ID x) fn (
    struct CTL_NODE* node = &ln->links;
    find_node ((void**) &node, ln->len, offsetof(struct CTL_NODE, next), offsetof(struct CTL_NODE, prev), i);

    CTL_TYPE_ID old = node->this;
    node->this = x;

    return old;
)


CTL_INLINE struct CTL_NODE* DEF_METHODE(ln, before, CTL_TYPE_NAME) (struct CTL_NODE* node, CTL_TYPE_ID x) fn (
    struct CTL_NODE* newNode;
    make_node ((void**) &newNode, sizeof(*newNode), 
               offsetof(struct CTL_NODE, next), node, 
               offsetof(struct CTL_NODE, prev), node->prev);

    newNode->this = x;

    return newNode;
)

CTL_INLINE struct CTL_NODE* DEF_METHODE(ln, after, CTL_TYPE_NAME) (struct CTL_NODE* node, CTL_TYPE_ID x) fn (
    struct CTL_NODE* newNode;
    make_node ((void**) &newNode, sizeof(*newNode),
               offsetof(struct CTL_NODE, next), node->next,
               offsetof(struct CTL_NODE, prev), node);

    newNode->this = x;

    return newNode;
)

CTL_INLINE struct CTL_NODE* DEF_METHODE(ln, add, CTL_TYPE_NAME) (struct CTL_STRUCT* l, size_t i, CTL_TYPE_ID x) fn (
    struct CTL_NODE* node = DEF_METHODE(ln, find, CTL_TYPE_NAME) (l, i);
    return DEF_METHODE(ln, before, CTL_TYPE_NAME) (node, x);
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(ln, rm, CTL_TYPE_NAME) (struct CTL_STRUCT* ls, size_t i) fn (
    struct CTL_NODE* node = &ls->links;
    find_node ((void**) &node, ls->len, offsetof(struct CTL_NODE, next), offsetof(struct CTL_NODE, prev), i);

    CTL_TYPE_ID old = node->this;
    del_node (node, offsetof(struct CTL_NODE, next), offsetof(struct CTL_NODE, prev));

    return old;
)


#include "undef-list.h"

#endif // CTL_NO_TEMPLATE

