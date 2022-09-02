#ifndef CTL_LINK_STD_H
#define CTL_LINK_STD_H

#include <stdint.h>


#define CTL_DECLARATION


#define CTL_TYPE int64_t
#define CTL_TYPE_NAME int64
#include "link.h"

#define CTL_TYPE uint64_t
#define CTL_TYPE_NAME uint64
#include "link.h"


#define CTL_TYPE int32_t
#define CTL_TYPE_NAME int32
#include "link.h"

#define CTL_TYPE uint32_t
#define CTL_TYPE_NAME uint32
#include "link.h"


#define CTL_TYPE int16_t
#define CTL_TYPE_NAME int16
#include "link.h"

#define CTL_TYPE uint16_t
#define CTL_TYPE_NAME uint16
#include "link.h"


#define CTL_TYPE int8_t
#define CTL_TYPE_NAME int8
#include "link.h"

#define CTL_TYPE uint8_t
#define CTL_TYPE_NAME uint8
#include "link.h"


#define CTL_TYPE float
#include "link.h"

#define CTL_TYPE double
#include "link.h"


#undef CTL_DECLARATION

#endif // CTL_LINK_STD_H
