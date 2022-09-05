#ifndef CTL_MACROS_H
#define CTL_MACROS_H

#include <stddef.h>


/**
 * Dynamique structure helpers
 */
typedef size_t FieldId;
#define field(P, O, T) *((T*)(((void*)P) + O))


#define CAT_2(A, B) A ## B
#define CAT_3(A, B, C) A ## B ## C

#define CAT_2_SEP(A, B, SEP) A ## SEP ## B
#define CAT_3_SEP(A, B, C, SEP) A ## SEP ## B ## SEP ## C


#define DEF_STRUCT(TYPE, SFX) CAT_2_SEP(TYPE, SFX, _)

#define DEF_CONSTRUCTOR(TYPE, STRUCT) CAT_3_SEP (make, TYPE, STRUCT, _)

#define DEF_DESTRUCTOR(TYPE, STRUCT) CAT_3_SEP (free, TYPE, STRUCT, _)

#define DEF_METHODE(STRUCT, ACTION, TYPE) CAT_3_SEP(STRUCT, ACTION, TYPE, _)

#endif // CTL_MACROS_H
