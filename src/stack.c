#define CTL_NO_TEMPLATE
#include "stack.h"

#include "macros.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


void make_stack (void* restrict nstack, size_t store, size_t len, size_t max, size_t size) {
    field (nstack, store, void*) = (size == 0) ? NULL : malloc (size);
    field (nstack, len, size_t) = 0;
    field (nstack, max, size_t) = size;
}

void grow_stack (void** restrict store, size_t* restrict max, size_t item) {
    *max = (*max == 0) ? 1 : *max * 2;
    *store = realloc (*store, *max * item);
}

void shrink_stack (void** restrict store, size_t* restrict max, size_t item) {
    *max /= 2;
    *store = realloc (*store, *max * item);
}


void stack_add (void* restrict store, void* restrict val, size_t* restrict len, size_t item, size_t i) {
    const size_t
        byteLen = *len * item,
        byteIndex = i * item;

    store += byteIndex;

    memmove (store + item, store, byteLen - byteIndex);
    memcpy (store, val, item);

    (*len)++;
}

void stack_rem (void* restrict ret, void* restrict store, size_t* restrict len, size_t item, size_t i) {
    const size_t
        byteLen = *len * item,
        byteIndex = i * item,
        byteNext = byteIndex + item;

    void* ptrAt = store + byteIndex;
    void* ptrNext = store + byteNext;

    memcpy (ret, ptrAt, item);
    memmove (ptrAt, ptrNext, byteLen - byteNext);

    (*len)--;
}

