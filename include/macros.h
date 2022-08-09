#ifndef CTL_MACROS_H
#define CTL_MACROS_H


#define CTL_STRUCT_LENGHT       len
#define CTL_STRUCT_MAX_CAPACITY max
#define CTL_STRUCT_OFFSET       off
#define CTL_STRUCT_STORAGE      store

#define len(S) ((S).CTL_STRUCT_LENGHT / (sizeof *((S).CTL_STRUCT_STORAGE)))


#define CAT_2(A, B) A ## B
#define CAT_3(A, B, C) A ## B ## C

#define CAT_2_SEP(A, B, SEP) A ## SEP ## B
#define CAT_3_SEP(A, B, C, SEP) A ## SEP ## B ## SEP ## C


#define DEF_STRUCT(TYPE, SFX) CAT_2_SEP(TYPE, SFX, _)

#define DEF_CONSTRUCTOR(TYPE, STRUCT) CAT_3_SEP (make, TYPE, STRUCT, _)

#define DEF_DESTRUCTOR(TYPE, STRUCT) CAT_3_SEP (free, TYPE, STRUCT, _)

#define DEF_METHODE(STRUCT, ACTION, TYPE) CAT_3_SEP(STRUCT, ACTION, TYPE, _)

#endif // CTL_MACROS_H
