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

typedef struct CBOff CBOff;
typedef struct CBLen CBLen;

typedef size_t Mask;

struct CBOff {
    size_t dest, src;
};

struct CBLen {
    size_t len;
    Mask mask;
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
static CBLen calc_buf_size (size_t required) {
    required--;

    required |= required >> 1;
    required |= required >> 2;
    required |= required >> 4;
    required |= required >> 8;
    required |= required >> 16;
    required |= required >> 32;

    required++;


    return (CBLen) { .len=required, .mask=len_mask (required) };
}


static CBOff move_b_cbuf (void* d, Mask dMask, size_t dOff, void* s, Mask sMask, size_t sOff, size_t size) {
    // Cycle the offset around the circular buffer
    dOff &= dMask;
    sOff &= sMask;

    size_t                                      //
        dEnd = (dOff + size) & dMask,           // Find the end of the chunks
        sEnd = (sOff + size) & sMask;           //

    // Cache the ends for returning
    const CBOff out = { .dest=dEnd, .src=sEnd };


    if (sEnd < size) {
        dEnd -= sEnd;                   //
        size -= sEnd;                   // Move the right most chunk
        memmove (d + dEnd, s, sEnd);    //

        if (dEnd < size) {                      //
            size -= dEnd;                       // Move the middle chunk
            memmove (d, s + dOff + size, dEnd); //
        }
    }

    // Move the the whole chunk or the first part
    memmove (d + dOff, s + sOff, size); 


    return out;
}

static CBOff move_f_cbuf (void* d, CBLen dMax, size_t dOff, void* s, CBLen sMax, size_t sOff, size_t size) {
    // Cycle the offset around the circular buffer
    dOff &= dMax.mask;
    sOff &= sMax.mask;

    size_t                                      //
        dStart = (dOff - size) & dMax.mask,     // Find the start of the chunks
        sStart = (sOff - size) & sMax.mask;     //

    // Cache the starts for returning
    const CBOff out = { .dest=dStart - 1, .src=sStart - 1 };


    size_t toCopy = sMax.len - sStart;

    if (toCopy < size) {
        size -= toCopy;                                 //
        memmove (d + dStart, s + sStart, toCopy);       //
                                                        // Move the last part of the chunk
        dStart += toCopy;                               //
        toCopy = dMax.len - dStart;                     //

        if (toCopy < size) {                            //
            size -= toCopy;                             // Move the middle part of the chunk
            memmove (d + dStart, s, toCopy);            //
        }
    }

    // Move remaining parts of the chunk
    memmove (d + (dOff - size), s + (sOff - size), size); 


    return out;
}

static CBOff copy_cbuf (void* restrict dest, void* restrict src, CBLen max, size_t off, size_t size) {
    // Cycle the offset around
    off &= max.mask;

    // Find the size of chunk to copy
    size_t toCopy = max.len - off;
    if ( toCopy > size ) toCopy = size;

    // Copy the chunks
    memcpy (dest, src + off, toCopy);
    memcpy (dest + toCopy, src, size - toCopy);


    return (CBOff) { .dest=size, .src=(off + size) & max.mask };
}

static struct CBOff copy_to_cbuf (void* restrict dest, CBLen max, size_t off, void* restrict src, size_t size) {
    // Cycle the offset around
    off &= max.mask;

    // Find the size of chunk to copy
    size_t toCopy = max.len - off;
    if ( toCopy > size ) toCopy = size;

    // Copy the chunks
    memcpy (dest + off, src, toCopy);
    memcpy (dest, src + toCopy, size - toCopy);


    return (CBOff) { .dest=(off + size) & max.mask, .src=size };
}


static void* realloc_cbuf (void** restrict buf, size_t* restrict off, CBLen oldSize, CBLen newSize) {
    // Reallocate the circular buffer with new size
    void* newBuf = realloc (*buf, newSize.len);
    if (!newBuf) abort ();

    // Move part of the deque to remove gaps created by the increase sized
    // when the beginning of the deque isn't at index '0'
    if (*off != 0) {
        size_t 
            toMove = oldSize.len - *off,
            newOff = newSize.len - toMove;

        memmove (newBuf + newOff, newBuf + *off, toMove);
        *off = newOff;
    }


    *buf = newBuf;
    return newBuf;
}


//
//      ======================================= API ============================================
//


void make_deq (void* ret, FieldId store, FieldId impl, size_t items, size_t ini) {
    CBLen bufSize = calc_buf_size (items * ini);

    field (ret, store, void*) = (bufSize.len > 0) ? malloc (bufSize.len) : NULL;
    field (ret, impl, struct MetaDeq) = (struct MetaDeq) {
        .mask = bufSize.mask,
        .max = bufSize.len,
        .len = 0,
        .off = 0
    };
}

void grow_deq (void** restrict store, size_t item, struct MetaDeq* restrict meta) {
    // Calculate the size of the grown buffer
    CBLen newMax;
    if (meta->max == 0) {
        newMax = calc_buf_size (2 * item);
    } else {
        newMax.len = meta->max << 1;
        newMax.mask = len_mask (newMax.len);
    }

    // Grow the buffer
    realloc_cbuf (store, &meta->off, (CBLen){ .len=meta->max, .mask=meta->mask }, newMax);

    // Update metadata
    meta->max = newMax.len;
    meta->mask = newMax.mask;
}

void shrink_deq (void** restrict store, struct MetaDeq* restrict meta) {
    // Calculate the size of the shrunk buffer
    CBLen newMax = { .len=meta->max >> 1, .mask=len_mask (newMax.len) };

    // Shrink the buffer
    realloc_cbuf (store, &meta->off, (CBLen){ .len=meta->max, .mask=meta->mask }, newMax);

    // Update metadata
    meta->max = newMax.len;
    meta->mask = newMax.mask;
}

void free_deq (void** restrict store, struct MetaDeq* impl) {
    free (*store);
    *store = NULL;

    *impl = (struct MetaDeq) { 0 };
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


