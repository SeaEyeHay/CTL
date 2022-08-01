#include "circ-buffer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


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
CBSize calc_buf_size (size_t required) {
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


void* alloc_cbuf (CBSize size) {
    assert ( is_pow_2 (size) && "Invalid buffer size!! - Use the calc_buf_size function." );
    return malloc (size);
}

void* realloc_cbuf (void** buf, size_t off, CBSize oldSize, CBSize newSize) {
    assert ( is_pow_2 (oldSize) && "Invalid old buffer size!! - Use the calc_buf_size function." );
    assert ( is_pow_2 (newSize) && "Invalid new buffer size!! - Use the calc_buf_size function." );

    void* newBuf = malloc (newSize);
    if (!newBuf) return NULL;

    copy_cbuf (newBuf, *buf, oldSize, off, (oldSize < newSize) ? oldSize : newSize);

    free (*buf);
    *buf = newBuf;

    return newBuf;
}

void free_cbuf (void** buf, CBSize size) {
    (void)size;

    free (*buf);
    *buf = NULL;
}


void* find_in_cbuf (void* buf, CBSize len, size_t off, size_t index) {
    assert ( is_pow_2 (len) && "Invalid buffer length!! - Use the calc_buf_size function." );

    const size_t mask = len_mask (len);
    const size_t i = (off + index) & mask;

    return buf + i;
}


struct CBOff move_b_cbuf (void* d, CBSize dLen, size_t dOff, void* s, CBSize sLen, size_t sOff, size_t size) {
    assert ( is_pow_2 (dLen) && "Invalid destination buffer length!! - Use the calc_buf_size function." );
    assert ( is_pow_2 (sLen) && "Invalid source buffer length!! - Use the calc_buf_size function." );


    const size_t dMask = len_mask (dLen), sMask = len_mask (sLen);

    dOff = dOff & dMask;
    sOff = sOff & sMask;


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

struct CBOff move_f_cbuf (void* d, CBSize dLen, size_t dOff, void* s, CBSize sLen, size_t sOff, size_t size) {
    assert ( is_pow_2 (dLen) && "Invalid destination buffer length!! - Use the calc_buf_size function." );
    assert ( is_pow_2 (sLen) && "Invalid source buffer length!! - Use the calc_buf_size function." );
    
    dOff = (dOff - size) & len_mask (dLen);
    sOff = (sOff - size) & len_mask (sLen);

    return move_b_cbuf (d, dLen, dOff, s, sLen, sOff, size);
}

struct CBOff copy_cbuf (void* dest, void* src, CBSize len, size_t off, size_t size) {
    assert ( is_pow_2 (len) && "Invalid source buffer length!! - Use the calc_buf_size function." );
    
    const size_t mask = len_mask (len);
    off &= mask;

    size_t toCopy = len - off;
    if ( toCopy > size ) toCopy = size;

    memcpy (dest, src + off, toCopy);
    memcpy (dest + toCopy, src, size - toCopy);

    return (struct CBOff) { .dest=size, .src=(off + size) & mask };
}

