#include "macros.h"


#ifndef CTL_TYPE_PFX
#define CTL_TYPE_PFX 
#endif

#ifndef CTL_TYPE
#define CTL_TYPE void
#define CTL_TYPE_SFX *
#endif

#ifndef CTL_TYPE_SFX
#define CTL_TYPE_SFX
#endif

#ifndef CTL_STRUCT_SFX
#define CTL_STRUCT_SFX ctl
#endif

#ifndef CTL_TYPE_NAME
#define CTL_TYPE_NAME CTL_TYPE
#endif


#define CTL_STRUCT DEF_STRUCT (CTL_TYPE_NAME, CTL_STRUCT_SFX)
#define CTL_TYPE_ID CTL_TYPE_PFX CTL_TYPE CTL_TYPE_SFX

#define CTL_GENERIC DEF_STRUCT (CTL_STRUCT, GENERIC)

#ifdef CTL_NODE_SFX
#define CTL_NODE DEF_STRUCT (CTL_TYPE_NAME, CTL_NODE_SFX)
#endif 


#ifdef CTL_DECLARATION // fn
#define fn(...) ;

#else
#define fn(...) { __VA_ARGS__ }

#endif // fn


#ifdef CTL_SHRUNK
#define shrink(FN, K) CAT_2_SEP (FN, K, _)

#else
#define shrink(FN, K) FN

#endif //




#if defined (CTL_IMPLEMENTATION) // CTL_INLINE
#define CTL_INLINE

#elif defined (CTL_DECLARATION)
#define CTL_INLINE extern

#else
#define CTL_INLINE static inline

#endif // CTL_INLINE

