#ifndef CTL_VECTOR_STD_H
#define CTL_VECTOR_STD_H

#include <stdint.h>


#define CTL_DECLARATION


#define CTL_TYPE int64_t
#define CTL_TYPE_NAME int64
#include "deque.h"

#define CTL_TYPE uint64_t
#define CTL_TYPE_NAME uint64
#include "deque.h"


#define CTL_TYPE int32_t
#define CTL_TYPE_NAME int32
#include "deque.h"

#define CTL_TYPE uint32_t
#define CTL_TYPE_NAME uint32
#include "deque.h"


#define CTL_TYPE int16_t
#define CTL_TYPE_NAME int16
#include "deque.h"

#define CTL_TYPE uint16_t
#define CTL_TYPE_NAME uint16
#include "deque.h"


#define CTL_TYPE int8_t
#define CTL_TYPE_NAME int8
#include "deque.h"

#define CTL_TYPE uint8_t
#define CTL_TYPE_NAME uint8
#include "deque.h"


#define CTL_TYPE float
#include "deque.h"

#define CTL_TYPE double
#include "deque.h"


#undef CTL_DECLARATION

#endif // CTL_VECTOR_STD_H
