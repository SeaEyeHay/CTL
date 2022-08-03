#ifndef CTL_VECTOR_H
#define CTL_VECTOR_H

#include "macros.h"

#include <stddef.h>


extern void make_ptr_vec (void* ret, size_t items, size_t length);

extern void resize_vec (void** store, size_t* max, size_t* offset, char shift);


extern void vec_get_ptr (void* ret, void* vec, size_t item, size_t lenght, size_t offset, size_t i);

extern void vec_set_ptr (void* val, void* vec, size_t item, size_t lenght, size_t offset, size_t i);

extern void vec_rem_ptr (void* vec, size_t item, size_t* length, size_t max, size_t* offset, size_t i);


#endif // CTL_VECTOR_H


//
// ================================================================================================
//


#ifndef CTL_NO_TEMPLATE

#ifndef CTL_TYPE_PFX
#define CTL_TYPE_PFX 
#endif

#ifndef CTL_TYPE
#define CTL_TYPE void
#define CTL_TYPE_SFX *
#endif

#ifndef CTL_TYPE_SFX
#define CTL_TYPE_SFX
#endif

#ifndef CTL_STRUCT_SFX
#define CTL_STRUCT_SFX v
#endif

#ifndef CTL_TYPE_NAME
#define CTL_TYPE_NAME CTL_TYPE
#endif


#define CTL_VECTOR DEF_STRUCT (CTL_TYPE_NAME, CTL_STRUCT_SFX)
#define CTL_TYPE_ID CTL_TYPE_PFX CTL_TYPE CTL_TYPE_SFX


#ifdef CTL_DECLARATION // fn
#define fn(...) ;

#else
#define fn(...) { __VA_ARGS__ }

#endif // fn

#if defined (CTL_IMPLEMENTATION) // CTL_INLINE
#define CTL_INLINE

#elif defined (CTL_DECLARATION)
#define CTL_INLINE extern

#else
#define CTL_INLINE static inline

#endif // CTL_INLINE


#ifndef CTL_IMPLEMENTATION

struct CTL_VECTOR {
    size_t CTL_STRUCT_LENGHT, CTL_STRUCT_OFFSET, CTL_STRUCT_MAX_CAPACITY;
    CTL_TYPE_ID* CTL_STRUCT_STORAGE;
};

#endif


CTL_INLINE struct CTL_VECTOR DEF_CONSTRUCTOR(CTL_TYPE_NAME, vec) (size_t iniSize) fn ( 
    struct CTL_VECTOR newVec;
    make_ptr_vec (&newVec, sizeof(CTL_TYPE_ID), iniSize);

    return newVec;
)


CTL_INLINE CTL_TYPE_ID DEF_METHODE(vec, get, CTL_TYPE_NAME) (struct CTL_VECTOR* vec, size_t i) fn (
    CTL_TYPE_ID ret;
    vec_get_ptr (&ret, vec->store, sizeof(CTL_TYPE_ID), vec->max, vec->off, i);

    return ret;
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(vec, set, CTL_TYPE_NAME) (struct CTL_VECTOR* vec, size_t i, CTL_TYPE_ID x) fn ( 
    CTL_TYPE_ID ret = DEF_METHODE(vec, get, CTL_TYPE_NAME) (vec, i);
    vec_set_ptr (&x, vec->store, sizeof(CTL_TYPE_ID), vec->max, vec->off, i);

    return ret;
)

CTL_INLINE void DEF_METHODE(vec, add, CTL_TYPE_NAME) (struct CTL_VECTOR* vec, size_t i, CTL_TYPE_ID x) fn (
    if (vec->len == vec->max) resize_vec (&vec->store, &vec->max, &vec->off, 1);
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(vec, rem, CTL_TYPE_NAME) (struct CTL_VECTOR* vec, size_t i) fn (
    CTL_TYPE_ID ret = DEF_METHODE(vec, get, CTL_TYPE_NAME) (vec, i);
    vec_rem_ptr (vec->store, sizeof(CTL_TYPE_ID), &vec->len, vec->max, &vec->off, i);

    if (vec->max >= 3*vec->len) resize_vec (&vec->store, &vec->max, &vec->off, -1);

    return ret;
)


#undef CTL_TYPE_PFX
#undef CTL_TYPE
#undef CTL_TYPE_SFX
#undef CTL_TYPE_ID

#undef CTL_TYPE_NAME

#undef CTL_STRUCT_SFX
#undef CTL_VECTOR

#undef fn
#undef CTL_INLINE

#endif // CTL_NO_TEMPLATE

