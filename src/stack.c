#define CTL_NO_TEMPLATE
#include "stack.h"

#include "macros.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


void make_stack (void* restrict nstack, size_t ini, size_t items, const struct StackGen* const restrict gen) {
    // Initial stack size in Bytes
    ini *= items;

    field (nstack, gen->store, void*) = (ini == 0) ? NULL : malloc (ini);
    field (nstack, gen->len, size_t) = 0;
    field (nstack, gen->max, size_t) = ini;
}

void grow_stack (void** restrict store, size_t* restrict max, size_t item) {
    *max = (*max == 0) ? 1 : *max * 2;
    *store = realloc (*store, *max * item);
}

void shrink_stack (void** restrict store, size_t* restrict max, size_t item) {
    *max /= 2;
    *store = realloc (*store, *max * item);
}


void slice_stack (void* restrict ret, void* restrict stk, const struct StackGen* restrict gen, size_t items, size_t a, size_t z) {
    field (ret, gen->store, void*) = field (stk, gen->store, void*) + a;

    size_t nlen = z - a;
    field (ret, gen->len, size_t) = nlen;
    field (ret, gen->max, size_t) = nlen;
}

void mov_stack (
        void* restrict dest, size_t destIndex,
        void* restrict src,  size_t srcIndex,
        const struct StackGen* restrict gen, size_t items, size_t n
) {
    void** restrict destStore = &field (dest, gen->store, void*);
    void** restrict srcStore  = &field (src, gen->store, void*);

    size_t* restrict destLen = &field (dest, gen->len, size_t);
    size_t* restrict srcLen  = &field (src, gen->len, size_t);

    size_t* restrict destMax = &field (dest, gen->max, size_t);
    size_t* restrict srcMax  = &field (src, gen->max, size_t);


    const size_t 
        toCopy          = n * items,
        destByteLen     = *destLen * items,
        destByteIndex   = destIndex * items,
        srcByteLen      = *srcLen * items,
        srcByteIndex    = srcIndex * items;

    void* destAt = *destStore + destByteIndex;
    void* srcAt  = *srcStore + srcByteIndex;

    if ( destByteLen + toCopy <= *destMax) grow_stack (destStore, destMax, items); 

    memmove (destAt + toCopy, destAt, destByteLen - destByteIndex);
    memcpy (destAt, srcAt, toCopy);
    memmove (srcAt, srcAt + toCopy, srcByteLen - toCopy - srcByteIndex);

    if ( *srcMax >= 3*(srcByteLen - toCopy) ) shrink_stack (srcStore, srcMax, items);

    (*destLen) += n;
    (*srcLen) -= n;
}


void stack_add_n (void* restrict stk, void* restrict val, const struct StackGen* restrict gen, size_t item, size_t i, size_t n) {
    size_t* restrict len = &field (stk, gen->len, size_t);
    size_t* restrict max = &field (stk, gen->max, size_t);

    void** restrict store = &field (stk, gen->store, void*);

    const size_t
        toCopy = item * n,
        byteLen = *len * item,
        byteIndex = i * item;

    void* at = *store + byteIndex;

    if ( byteLen + toCopy <= *max ) grow_stack (store, max, item); 

    memmove (at + toCopy, at, byteLen - byteIndex);
    memcpy (at, val, toCopy);

    (*len) += n;
}

void stack_rm_n (void* restrict ret, void* restrict stk, const struct StackGen* restrict gen, size_t items, size_t i, size_t n) {
    size_t* len = &field (stk, gen->len, size_t);
    size_t* max = &field (stk, gen->max, size_t);

    void** store = &field (stk, gen->store, void*);

    const size_t
        toCopy = items * n,
        byteLen = *len * items,
        byteIndex = i * items,
        byteNext = byteIndex + toCopy;


    void* ptrAt = store + byteIndex;

    if (ret != NULL) memcpy (ret, ptrAt, items);
    memmove (ptrAt, store + byteNext, byteLen - byteNext);

    if ( *max >= 3*(byteLen - toCopy) ) shrink_stack (store, max, items);

    (*len) -= n;
}

