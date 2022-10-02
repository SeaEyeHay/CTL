#ifndef CTL_STACK_H
#define CTL_STACK_H

#include <stddef.h>
#include <assert.h>

#include "generics.h"


extern void make_stack (void* restrict nstack, size_t ini, size_t items, const struct StackGen* restrict gen);
//extern void make_stack_rooted ();

extern void grow_stack (void** restrict store, size_t* restrict max, size_t item);
//extern void grow_stack_rooted ();

extern void shrink_stack (void** restrict store, size_t* restrict max, size_t item);
//extern void shrink_stack_rooted (void* restrict store, size_t* restrict max);

extern void slice_stack (void* restrict ret, void* restrict stk, const struct StackGen* restrict gen, 
                         size_t items, size_t a, size_t z);

extern void mov_stack (void* restrict dest, size_t destIndex,
                       void* restrict src,  size_t srcIndex,
                       const struct StackGen* restrict gen, size_t items, size_t n);

extern void stack_add_n (void* restrict stack, void* restrict val, const struct StackGen* restrict gen, 
                         size_t item, size_t i, size_t n);

extern void stack_rm_n (void* restrict ret, void* restrict stack, const struct StackGen* restrict gen, 
                        size_t items, size_t i, size_t n);


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

static const struct StackGen CTL_GENERIC = {
    .store=offsetof (struct CTL_STRUCT, store),
    .len=offsetof (struct CTL_STRUCT, len),
    .max=offsetof (struct CTL_STRUCT, max)
};

#endif // CTL_IMPLEMENTATION


CTL_INLINE struct CTL_STRUCT DEF_CONSTRUCTOR(CTL_TYPE_NAME, stk) (size_t ini) fn (
    struct CTL_STRUCT newStack;
    make_stack (&newStack, ini, sizeof(CTL_TYPE_ID), &CTL_GENERIC);

    return newStack;
)


CTL_INLINE CTL_TYPE_ID DEF_METHODE(stk, get, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t i) fn (
    assert (i < stk->len);
    return stk->store[i];
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(stk, set, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t i, CTL_TYPE_ID x) fn (
    assert (i < stk->len);

    CTL_TYPE_ID removed = stk->store[i];
    stk->store[i] = x;

    return removed;
)


CTL_INLINE const struct CTL_STRUCT DEF_METHODE(stk, slice, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t start, size_t end) fn (
    assert (start < end);
    assert (end < stk->len);

    struct CTL_STRUCT newSlice;
    slice_stack (&newSlice, stk, &CTL_GENERIC, sizeof(CTL_TYPE_ID), start, end);

    return newSlice;
)

CTL_INLINE void DEF_METHODE(stk, mov, CTL_TYPE_NAME) (
        struct CTL_STRUCT* restrict dest, size_t destIndex,
        struct CTL_STRUCT* restrict src,  size_t srcIndex,
        size_t n
) fn (
    assert (destIndex <= dest->len);
    assert (srcIndex < src->len);
    assert (srcIndex + n < src->len);

    mov_stack (dest, destIndex, src, srcIndex, &CTL_GENERIC, sizeof(CTL_TYPE_ID), n);
)

CTL_INLINE void DEF_METHODE(stk, catslice, CTL_TYPE_NAME) (
        struct CTL_STRUCT* restrict dest, size_t destIndex, 
        struct CTL_STRUCT* restrict src,  size_t start, size_t end
) fn (
    assert (destIndex < dest->len);
    assert (destIndex + start - end < dest->len);
    assert (end < src->len);
    assert (start < end);

    struct CTL_STRUCT tempSlice;
    slice_stack (&tempSlice, src, &CTL_GENERIC, sizeof(CTL_TYPE_ID), start, end);

    stack_add_n (dest, tempSlice.store, &CTL_GENERIC, sizeof(CTL_TYPE_ID), destIndex, tempSlice.len);
)


CTL_INLINE void DEF_METHODE(stk, add, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t i, CTL_TYPE_ID x) fn (
    assert (i <= stk->len);
    stack_add_n (stk, &x, &CTL_GENERIC, sizeof x, i, 1);
)

CTL_INLINE void DEF_METHODE(stk, nadd, CTL_TYPE_NAME) (struct CTL_STRUCT* restrict stk, size_t i, size_t n, CTL_TYPE_ID x[]) fn (
    assert (i <= stk->len);
    stack_add_n (stk, x, &CTL_GENERIC, sizeof(CTL_TYPE_ID), i, n);
)

CTL_INLINE void DEF_METHODE(stk, cat, CTL_TYPE_NAME) (struct CTL_STRUCT* restrict stk, size_t i, struct CTL_STRUCT* restrict xs) fn (
    assert (i <= stk->len);
    stack_add_n (stk, xs->store, &CTL_GENERIC, sizeof(CTL_TYPE_ID), i, xs->len);
)


CTL_INLINE CTL_TYPE_ID DEF_METHODE(stk, rm, CTL_TYPE_NAME) (struct CTL_STRUCT* stk, size_t i) fn (
    assert (i < stk->len);

    CTL_TYPE_ID removed;
    stack_rm_n (&removed, stk, &CTL_GENERIC, sizeof removed, i, 1);

    return removed;
)

CTL_INLINE void DEF_METHODE(stk, nrm, CTL_TYPE_NAME) (
        CTL_TYPE_ID* restrict dest, struct CTL_STRUCT* restrict src, 
        size_t i, size_t n
) fn (
    assert ( i < src->len );
    assert ( (i + n) < src->len );

    stack_rm_n (dest, src, &CTL_GENERIC, sizeof *dest, i, n);
)


#include "undef-list.h"

#endif // CTL_NO_TEMPLATE

