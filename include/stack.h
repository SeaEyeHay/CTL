#ifndef CTL_STACK_H
#define CTL_STACK_H

#include <stddef.h>
#include <assert.h>


extern void make_stack (void* restrict nstack, size_t store, size_t len, size_t max, size_t size);
//extern void make_stack_rooted ();

extern void grow_stack (void** restrict store, size_t* restrict max, size_t item);
//extern void grow_stack_rooted ();

extern void shrink_stack (void** restrict store, size_t* restrict max, size_t item);
//extern void shrink_stack_rooted (void* restrict store, size_t* restrict max);


extern void stack_add (void* restrict store, void* restrict val, size_t* restrict len, size_t item, size_t i);

extern void stack_rem (void* restrict ret, void* restrict store, size_t* restrict len, size_t item, size_t i);


#endif // CTL_STACK_H


//
// ================================================================================================
//


#ifndef CTL_NO_TEMPLATE

#define CTL_STRUCT_SFX k
#include "template.h"


#ifndef CTL_IMPLEMENTATION

struct CTL_STRUCT {
    size_t len, max;
    CTL_TYPE_ID* store;
};

#endif // CTL_IMPLEMENTATION


CTL_INLINE struct CTL_STRUCT DEF_CONSTRUCTOR(CTL_TYPE_NAME, stack) (size_t initSize) fn (
    static const size_t
        storeOff = offsetof(struct CTL_STRUCT, store),
        lenOff   = offsetof(struct CTL_STRUCT, len),
        maxOff   = offsetof(struct CTL_STRUCT, max);

    struct CTL_STRUCT newStack;
    shrink(make_stack, rooted) (&newStack, storeOff, lenOff, maxOff, sizeof(CTL_TYPE_ID));

    return newStack;
)


CTL_INLINE CTL_TYPE_ID DEF_METHODE(stack, get, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t i) fn (
    assert (i < stk->len);
    return stk->store[i];
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(stack, set, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t i, CTL_TYPE_ID x) fn (
    assert (i < stk->len);

    CTL_TYPE_ID removed = stk->store[i];
    stk->store[i] = x;

    return removed;
)


CTL_INLINE void DEF_METHODE(stack, add, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t i, CTL_TYPE_ID x) fn (
    assert (i <= stk->len);

    if (stk->max == stk->len) grow_stack ((void**) &stk->store, &stk->max, sizeof x);
    stack_add (stk->store, &x, &stk->len, sizeof x, i);
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(stack, rm, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t i) fn (
    assert (i < stk->len);

    if (stk->max >= 3 * stk->len) shrink_stack ((void**) &stk->store, &stk->max, sizeof(CTL_TYPE_ID));

    CTL_TYPE_ID removed;
    stack_rem (&removed, stk->store, &stk->len, sizeof(CTL_TYPE_ID), i);

    return removed;
)


#include "undef-list.h"

#endif // CTL_NO_TEMPLATE

