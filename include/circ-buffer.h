#ifndef CTL_CIRC_BUFFER_H
#define CTL_CIRC_BUFFER_H

#include <stddef.h>


typedef size_t CBSize;

extern CBSize calc_buf_size (size_t required);


extern void* alloc_cbuf (CBSize size);

extern void* realloc_cbuf (void** buf, size_t off, CBSize oldSize, CBSize newSize);

extern void free_cbuf (void** buf, CBSize size);


extern void* find_in_cbuf (void* buf, CBSize len, size_t off, size_t index);


struct CBOff {
    size_t dest, src;
};

extern struct CBOff move_b_cbuf (void* d, CBSize dlen, size_t doff, void* s, CBSize slen, size_t soff, size_t size);

extern struct CBOff move_f_cbuf (void* d, CBSize dlen, size_t doff, void* s, CBSize slen, size_t soff, size_t size);

extern struct CBOff copy_cbuf (void* dest, void* src, CBSize len, size_t off, size_t size);


#endif // CTL_CIRC_BUFFER_H
