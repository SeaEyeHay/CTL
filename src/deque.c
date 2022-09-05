#define CTL_NO_TEMPLATE
#include "deque.h"

#include "macros.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


//
//      ======================================= CIRCULAR BUFFER ======================================= 
//

typedef size_t CBSize;

struct CBOff {
    size_t dest, src;
};


#ifndef NDEBUG

/**
 * Credit to : https://stackoverflow.com/a/600306
 */
static inline bool is_pow_2 (size_t n) {
    return (n & (n - 1)) == 0;
}

#endif // NDEBUG


static inline size_t len_mask (size_t length) {
    return length - 1;
}


/**
 * Bit Twiddling Hacks
 * Credit to : https://stackoverflow.com/a/466242
 */
static CBSize calc_buf_size (size_t required) {
    required--;

    required |= required >> 1;
    required |= required >> 2;
    required |= required >> 4;
    required |= required >> 8;
    required |= required >> 16;
    required |= required >> 32;

    required++;


    return required;
}



static void* find_in_cbuf (void* buf, CBSize len, size_t off, size_t index) {
    assert ( is_pow_2 (len) && "Invalid buffer length!! - Use the calc_buf_size function." );

    const size_t mask = len_mask (len);
    const size_t i = (off + index) & mask;

    return buf + i;
}


static struct CBOff move_b_cbuf (void* d, CBSize dLen, size_t dOff, void* s, CBSize sLen, size_t sOff, size_t size) {
    assert ( is_pow_2 (dLen) && "Invalid destination buffer length!! - Use the calc_buf_size function." );
    assert ( is_pow_2 (sLen) && "Invalid source buffer length!! - Use the calc_buf_size function." );


    // Cycle the offset around the circle
    const size_t dMask = len_mask (dLen), sMask = len_mask (sLen);

    dOff = dOff & dMask;
    sOff = sOff & sMask;


    // Decide which buffer is shorter and which is longer
    size_t longer = dLen - dOff, shorter = sLen - sOff;

    if ( longer < shorter ) {
        size_t temp = longer;
        longer = shorter;
        shorter = temp;
    }


    if ( size > shorter ) {
        memmove (d + dOff, s + sOff, shorter);
        dOff = (dOff + shorter) & dMask;
        sOff = (sOff + shorter) & sMask;

        longer -= shorter;
        size -= shorter;
    }

    if ( size > longer ) {
        memmove (d + dOff, s + sOff, longer);
        dOff = (dOff + longer) & dMask;
        sOff = (sOff + longer) & sMask;

        size -= longer;
    }

    memmove (d + dOff, s + sOff, size);
    dOff = (dOff + size) & dMask;
    sOff = (sOff + size) & sMask;


    return (struct CBOff) { .dest=dOff, .src=sOff };
}

static struct CBOff move_f_cbuf (void* d, CBSize dLen, size_t dOff, void* s, CBSize sLen, size_t sOff, size_t size) {
    assert ( is_pow_2 (dLen) && "Invalid destination buffer length!! - Use the calc_buf_size function." );
    assert ( is_pow_2 (sLen) && "Invalid source buffer length!! - Use the calc_buf_size function." );
    
    dOff = dOff - size; 
    sOff = sOff - size; 

    move_b_cbuf (d, dLen, dOff, s, sLen, sOff, size);


    return (struct CBOff) { .dest=dOff, .src=sOff };
}

static struct CBOff copy_cbuf (void* restrict dest, void* restrict src, CBSize len, size_t off, size_t size) {
    assert ( is_pow_2 (len) && "Invalid source buffer length!! - Use the calc_buf_size function." );
    
    const size_t mask = len_mask (len);
    off &= mask;

    size_t toCopy = len - off;
    if ( toCopy > size ) toCopy = size;

    memcpy (dest, src + off, toCopy);
    memcpy (dest + toCopy, src, size - toCopy);


    return (struct CBOff) { .dest=size, .src=(off + size) & mask };
}

static struct CBOff copy_to_cbuf (void* restrict dest, CBSize len, size_t off, void* restrict src, size_t size) {
    assert ( is_pow_2 (len) && "Invalid destination buffer length!! - Use the calc_buf_size function." );

    const size_t mask = len_mask (len);
    off &= mask;

    size_t toCopy = len - off;
    if ( toCopy > size ) toCopy = size;

    memcpy (dest + off, src, toCopy);
    memcpy (dest, src + toCopy, size - toCopy);


    return (struct CBOff) { .dest=(off + size) & mask, .src=size };
}


static void* alloc_cbuf (CBSize size) {
    assert ( is_pow_2 (size) && "Invalid buffer size!! - Use the calc_buf_size function." );
    return malloc (size);
}

static void* realloc_cbuf (void** buf, size_t off, CBSize oldSize, CBSize newSize) {
    assert ( is_pow_2 (oldSize) && "Invalid old buffer size!! - Use the calc_buf_size function." );
    assert ( is_pow_2 (newSize) && "Invalid new buffer size!! - Use the calc_buf_size function." );

    void* newBuf = malloc (newSize);
    if (!newBuf) return NULL;

    copy_cbuf (newBuf, *buf, oldSize, off, (oldSize < newSize) ? oldSize : newSize);

    free (*buf);
    *buf = newBuf;


    return newBuf;
}


//
//      ======================================= API ============================================
//


void make_deq (void* ret, FieldId store, FieldId impl, size_t items, size_t ini) {
    CBSize bufSize = calc_buf_size (items * ini);

    field (ret, store, void*) = (bufSize > 0) ? malloc (bufSize) : NULL;
    field (ret, impl, struct MetaDeque) = (struct MetaDeque) {
        .mask = len_mask (bufSize),
        .len = 0,
        .off = 0
    };
}

void grow_deq (void** restrict store, size_t item, size_t* restrict max, size_t* restrict off) {
    
    CBSize newMax;
    if (*max == 0) {
        newMax = calc_buf_size (2 * item);
    } else {
        newMax = *max << 1;
    }

    realloc_cbuf (store, *off, *max, newMax);

    *max = newMax;
    *off = 0;
}

void shrink_deq (void** restrict store, size_t* restrict max, size_t* restrict off) {
    CBSize newMax = *max >> 1;
    realloc_cbuf (store, *off, *max, newMax);

    *max = newMax;
    *off = 0;
}

void free_deq (void** restrict store, struct MetaDeque* impl) {
    free (*store);
    *store = NULL;

    *impl = (struct MetaDeque) { 0 };
}


void deq_get (void* restrict ret, void* restrict deq, size_t item, size_t lenght, size_t offset, size_t i) {
    i *= item;
    copy_cbuf (ret, deq, lenght, offset + i, item);
}

void deq_set (void* restrict val, void* restrict deq, size_t item, size_t length, size_t offset, size_t i) {
    i *= item;
    copy_to_cbuf (deq, length, offset + i, val, item);
}

void deq_add (
    void* restrict deq, size_t item, size_t* restrict length, size_t max, size_t* restrict offset, 
    size_t i
) {
    i *= item;

    if ( i < *length/2 ) {
        *offset = move_f_cbuf (deq, max, *offset + i, deq, max, *offset + i + item, i + item).dest;
    } else {
        const size_t toMove = *length - i;
        move_b_cbuf (deq, max, *offset + i + item, deq, max, *offset + i, toMove);
    }

    *length += item;
}

void deq_rem (
    void* restrict deq, size_t item, size_t* restrict length, size_t max, size_t* restrict offset, 
    size_t i
) {
    i *= item;

    if ( i < *length/2 ) {
        *offset = move_f_cbuf (deq, max, *offset + i + item, deq, max, *offset + i, i).dest;
    } else {
        const size_t toMove = *length - (i + item);
        move_b_cbuf (deq, max, *offset + i, deq, max, *offset + i + item, toMove);
    }

    *length -= item;
}


void deq_push (
    void* restrict deq, void* restrict val, size_t item, size_t* restrict length, 
    size_t max, size_t offset
) {
    move_b_cbuf (deq, max, offset + *length, val, item, 0, item);
    *length += item;
}

void deq_push_front (
    void* restrict deq, void* restrict val, size_t item, size_t* restrict length, size_t max, 
    size_t* restrict offset
) {
    move_f_cbuf (deq, max, *offset, val, item, 0, item);

    *offset = *offset - item & len_mask (max);
    *length += item;
}


void deq_pop (
    void* restrict ret, void* restrict deq, size_t item, size_t* restrict length, size_t max, 
    size_t offset
) {
    *length -= item;
    copy_cbuf (ret, deq, max, offset + *length, item);
}

void deq_pop_front (
    void* restrict ret, void* restrict deq, size_t item, size_t* restrict length, size_t max, 
    size_t* restrict offset
) {
    copy_cbuf (ret, deq, max, *offset, item);

    *offset = *offset + item & len_mask (max);
    *length -= item;
}


//
//      ======================================= ITERATOR ============================================
//


