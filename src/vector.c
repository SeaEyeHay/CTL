#define CTL_NO_TEMPLATE
#include "vector.h"
#include "circ-buffer.h"


struct CtlGenericVec {
    size_t len, offset, max;
    void* store;
};


void make_ptr_vec (void* ret, size_t items, size_t length) {
    CBSize bufSize = calc_buf_size (items * length);

    struct CtlGenericVec newVec = { .len=0, .offset=0 };
    newVec.max = bufSize;

    newVec.store = (bufSize > 0) ? alloc_cbuf (bufSize) : NULL;


    *((struct CtlGenericVec*)ret) = newVec;
}

void resize_vec (void** store, size_t* max, size_t* offset, char shift) {
    CBSize newMax = *max << shift;
    if (newMax < 1) newMax = 2;

    realloc_cbuf (store, *offset, *max, newMax);

    *max = newMax;
    *offset = 0;
}


void vec_get_ptr (void* ret, void* vec, size_t item, size_t lenght, size_t offset, size_t i) {
    i *= item;
    copy_cbuf (ret, vec, lenght, offset + i, item);
}

void vec_set_ptr (void* val, void* vec, size_t item, size_t lenght, size_t offset, size_t i) {
    i *= item;
    move_b_cbuf (vec, lenght, offset + i, val, item, 0, item);
}

void vec_rem_ptr (void* vec, size_t item, size_t* length, size_t max, size_t* offset, size_t i) {
    i *= item;

    if ( i < *length/2 ) {
        *offset = move_f_cbuf (vec, max, *offset + i + item, vec, max, *offset + i, i).dest;
    } else {
        const size_t toMove = *length - i;
        move_b_cbuf (vec, max, *offset + i, vec, max, *offset + i + item, toMove);
    }

    *length -= item;
}
