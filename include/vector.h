#ifndef CTL_VECTOR_H
#define CTL_VECTOR_H

#include <stddef.h>


extern void make_ptr_vec (void* ret, size_t items, size_t length);

extern void grow_vec (void** restrict store, size_t item, size_t* restrict max, size_t* restrict off);

extern void shrink_vec (void** restrict store, size_t* restrict max, size_t* restrict off);

extern void free_vec (void** restrict store, size_t* restrict len, size_t* restrict max, size_t* restrict off);


extern void vec_get_ptr (void* restrict ret, void* restrict vec, size_t item, size_t max, size_t offset, size_t i);

extern void vec_set_ptr (void* restrict val, void* restrict vec, size_t item, size_t max, size_t offset, size_t i);

extern void vec_add_ptr (void* restrict vec, size_t item, size_t* restrict length, size_t max, 
                         size_t* restrict offset, size_t i);

extern void vec_rem_ptr (void* restrict vec, size_t item, size_t* restrict length, size_t max, 
                         size_t* restrict offset, size_t i);


extern void vec_push_ptr (void* restrict vec, void* restrict val, size_t item, size_t* restrict length, 
                          size_t max, size_t offset);

extern void vec_push_front_ptr (void* restrict vec, void* restrict val, size_t item,
                                size_t* restrict length, size_t max, size_t* restrict offset);


extern void vec_pop_ptr (void* restrict ret, void* restrict vec, size_t item, size_t* restrict length, 
                         size_t max, size_t offset);

extern void vec_pop_front_ptr (void* restrict ret, void* restrict vec, size_t item, 
                               size_t* restrict length, size_t max, size_t* offset);


#endif // CTL_VECTOR_H


//
// ================================================================================================
//


#ifndef CTL_NO_TEMPLATE

#define CTL_STRUCT_SFX v
#include "template.h"


#ifndef CTL_IMPLEMENTATION

struct CTL_STRUCT {
    size_t CTL_STRUCT_LENGHT, CTL_STRUCT_OFFSET, CTL_STRUCT_MAX_CAPACITY;
    CTL_TYPE_ID* CTL_STRUCT_STORAGE;
};

#endif


CTL_INLINE struct CTL_STRUCT DEF_CONSTRUCTOR(CTL_TYPE_NAME, vec) (size_t iniSize) fn ( 
    struct CTL_STRUCT newVec;
    make_ptr_vec (&newVec, sizeof(CTL_TYPE_ID), iniSize);

    return newVec;
)

CTL_INLINE void DEF_DESTRUCTOR(CTL_TYPE_NAME, vec) (struct CTL_STRUCT* vec) fn (
    free_vec ((void**)&vec->store, &vec->len, &vec->max, &vec->off);
)


CTL_INLINE CTL_TYPE_ID DEF_METHODE(vec, get, CTL_TYPE_NAME) (struct CTL_STRUCT* vec, size_t i) fn (
    CTL_TYPE_ID ret;
    vec_get_ptr (&ret, vec->store, sizeof(CTL_TYPE_ID), vec->max, vec->off, i);

    return ret;
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(vec, set, CTL_TYPE_NAME) (struct CTL_STRUCT* vec, size_t i, CTL_TYPE_ID x) fn ( 
    CTL_TYPE_ID ret = DEF_METHODE(vec, get, CTL_TYPE_NAME) (vec, i);
    vec_set_ptr (&x, vec->store, sizeof(CTL_TYPE_ID), vec->max, vec->off, i);

    return ret;
)

CTL_INLINE void DEF_METHODE(vec, add, CTL_TYPE_NAME) (struct CTL_STRUCT* vec, size_t i, CTL_TYPE_ID x) fn (
    if (vec->len == vec->max) grow_vec ((void**)&vec->store, sizeof(CTL_TYPE_ID), &vec->max, &vec->off);

    vec_add_ptr (vec->store, sizeof(CTL_TYPE_ID), &vec->len, vec->max, &vec->off, i);
    vec_set_ptr (&x, vec->store, sizeof(CTL_TYPE_ID), vec->max, vec->off, i);
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(vec, rem, CTL_TYPE_NAME) (struct CTL_STRUCT* vec, size_t i) fn (
    CTL_TYPE_ID ret = DEF_METHODE(vec, get, CTL_TYPE_NAME) (vec, i);
    vec_rem_ptr (vec->store, sizeof(CTL_TYPE_ID), &vec->len, vec->max, &vec->off, i);

    if (vec->max >= 3*vec->len) shrink_vec ((void**)&vec->store, &vec->max, &vec->off);

    return ret;
)


CTL_INLINE void DEF_METHODE(vec, push, CTL_TYPE_NAME) (struct CTL_STRUCT* vec, CTL_TYPE_ID x) fn (
    if (vec->len == vec->max) grow_vec ((void**)&vec->store, sizeof(CTL_TYPE_ID), &vec->max, &vec->off);
    vec_push_ptr (vec->store, &x, sizeof(CTL_TYPE_ID), &vec->len, vec->max, vec->off);
)

CTL_INLINE void DEF_METHODE(vec, push_front, CTL_TYPE_NAME) (struct CTL_STRUCT* vec, CTL_TYPE_ID x) fn (
    if (vec->len == vec->max) grow_vec ((void**)&vec->store, sizeof(CTL_TYPE_ID), &vec->max, &vec->off);
    vec_push_front_ptr (vec->store, &x, sizeof(CTL_TYPE_ID), &vec->len, vec->max, &vec->off);
)


CTL_INLINE CTL_TYPE_ID DEF_METHODE(vec, pop, CTL_TYPE_NAME) (struct CTL_STRUCT* vec) fn (
    CTL_TYPE_ID ret;
    vec_pop_ptr (&ret, vec->store, sizeof(CTL_TYPE_ID), &vec->len, vec->max, vec->off);

    if (vec->max >= 3*vec->len) shrink_vec ((void**)&vec->store, &vec->max, &vec->off);

    return ret;
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(vec, pop_front, CTL_TYPE_NAME) (struct CTL_STRUCT* vec) fn (
    CTL_TYPE_ID ret;
    vec_pop_front_ptr (&ret, vec->store, sizeof(CTL_TYPE_ID), &vec->len, vec->max, &vec->off);

    if (vec->max >= 3*vec->len) shrink_vec ((void**)&vec->store, &vec->max, &vec->off);

    return ret;
)


#include "undef-list.h"

#endif // CTL_NO_TEMPLATE

