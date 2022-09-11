#ifndef CTL_VECTOR_H
#define CTL_VECTOR_H

#include <stddef.h>

#include "macros.h"


/**
 *      METADATA
 */

struct MetaDeq {
    size_t max, mask, len, off;
};

#define deq_len(D) (D)->meta.len
#define deq_max(D) (D)->meta.max / sizeof *( (D)->store )


/**
 *      BACKEND
 */

extern void make_deq (void* ret, FieldId store, FieldId impl, size_t items, size_t ini);

extern void grow_deq (void** restrict store, size_t item, struct MetaDeq* meta);

extern void shrink_deq (void** restrict store, struct MetaDeq* restrict meta);

extern void free_deq (void** restrict store, struct MetaDeq* impl);


extern void deq_get (void* restrict ret, void* restrict deq, size_t item, struct MetaDeq* meta, size_t i);

extern void deq_set (void* restrict val, void* restrict deq, size_t item, struct MetaDeq* restrict meta, size_t i);

extern void deq_add (void** restrict deq, size_t item, struct MetaDeq* restrict meta, size_t i);

extern void deq_rem (void** restrict deq, size_t item, struct MetaDeq* restrict meta, size_t i);


#endif // CTL_VECTOR_H


//
// ================================================================================================
//


#ifndef CTL_NO_TEMPLATE

#define CTL_STRUCT_SFX d
#include "template.h"


#ifndef CTL_IMPLEMENTATION

struct CTL_STRUCT {
    CTL_TYPE_ID* store;
    struct MetaDeq meta;
};

#endif


CTL_INLINE struct CTL_STRUCT DEF_CONSTRUCTOR(CTL_TYPE_NAME, deq) (size_t ini) fn ( 
    struct CTL_STRUCT newDeq;
    make_deq (&newDeq, fid(store), fid(meta), sizeof(CTL_TYPE_ID), ini);

    return newDeq;
)

CTL_INLINE void DEF_DESTRUCTOR(CTL_TYPE_NAME, deq) (struct CTL_STRUCT* deq) fn (
    free_deq ((void**) &deq->store, &deq->meta);
)


CTL_INLINE CTL_TYPE_ID DEF_METHODE(deq, get, CTL_TYPE_NAME) (struct CTL_STRUCT* deq, size_t i) fn (
    CTL_TYPE_ID ret;
    deq_get (&ret, deq->store, sizeof(CTL_TYPE_ID), &deq->meta, i);

    return ret;
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(deq, set, CTL_TYPE_NAME) (struct CTL_STRUCT* deq, size_t i, CTL_TYPE_ID x) fn ( 
    CTL_TYPE_ID ret = DEF_METHODE(deq, get, CTL_TYPE_NAME) (deq, i);
    deq_set (&x, deq->store, sizeof x, &deq->meta, i);

    return ret;
)

CTL_INLINE void DEF_METHODE(deq, add, CTL_TYPE_NAME) (struct CTL_STRUCT* deq, size_t i, CTL_TYPE_ID x) fn (
    deq_add ((void**) &deq->store, sizeof x, &deq->meta, i);
    deq_set (&x, deq->store, sizeof x, &deq->meta, i);
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(deq, rem, CTL_TYPE_NAME) (struct CTL_STRUCT* deq, size_t i) fn (
    CTL_TYPE_ID ret = DEF_METHODE(deq, get, CTL_TYPE_NAME) (deq, i);
    deq_rem ((void**) &deq->store, sizeof(CTL_TYPE_ID), &deq->meta, i);

    return ret;
)


CTL_INLINE void DEF_METHODE(deq, push, CTL_TYPE_NAME) (struct CTL_STRUCT* deq, CTL_TYPE_ID x) fn (
    DEF_METHODE(deq, add, CTL_TYPE_NAME) (deq, deq->meta.len, x);
)

CTL_INLINE void DEF_METHODE(deq, push_front, CTL_TYPE_NAME) (struct CTL_STRUCT* deq, CTL_TYPE_ID x) fn (
    DEF_METHODE(deq, add, CTL_TYPE_NAME) (deq, 0, x);
)


CTL_INLINE CTL_TYPE_ID DEF_METHODE(deq, pop, CTL_TYPE_NAME) (struct CTL_STRUCT* deq) fn (
    return DEF_METHODE(deq, rem, CTL_TYPE_NAME) (deq, deq->meta.len - 1);
)

CTL_INLINE CTL_TYPE_ID DEF_METHODE(deq, pop_front, CTL_TYPE_NAME) (struct CTL_STRUCT* deq) fn (
    return DEF_METHODE(deq, rem, CTL_TYPE_NAME) (deq, 0);
)


#include "undef-list.h"

#endif // CTL_NO_TEMPLATE

