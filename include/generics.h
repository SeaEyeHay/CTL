#ifndef CTL_GENERICS_H
#define CTL_GENERICS_H

#include <stddef.h>


struct StackGen{
    size_t store, len, max;
};

struct DequeGen {
    struct StackGen left, right;
};

#endif // CTL_GENERICS_H
