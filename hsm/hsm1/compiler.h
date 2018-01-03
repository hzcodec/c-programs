//============================================================================
//
// $URL: svn+ssh://ristna.unjo.local/var/svn/220_06_005_COMMON/trunk/include/compiler.h $
// $Revision: 11805 $
// $Date: 2016-11-28 11:46:07 +0100 (mån, 28 nov 2016) $
// $Author: andreas.fritiofson $
//
// Description :
//
// Copyright   : UNJO AB
//
//============================================================================

#ifndef COMPILER_H_
#define COMPILER_H_

#if (defined __GNUC__)

#include "fixpoint.h"

#define REORDER_BARRIER() asm volatile("" ::: "memory")
#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
#define UNUSED(x) __unused_##x __attribute__ ((unused))
#ifndef __always_inline
#define __always_inline inline __attribute__ ((always_inline))
#endif
#define __noreturn __attribute__ ((noreturn))
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})
#ifndef MAX
#define MAX(a,b) \
		({ typeof (a) _a = (a); typeof (b) _b = (b); \
		_a > _b ? _a : _b; })
#endif
#ifndef MIN
#define MIN(a,b) \
		({ typeof (a) _a = (a); typeof (b) _b = (b); \
		_a < _b ? _a : _b; })
#endif

#elif (defined __IAR_SYSTEMS_ICC__)

#define REORDER_BARRIER() asm ("")
#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x)) /* Not likely to work */
#define UNUSED(x) __unused_##x
#define __always_inline _Pragma("inline=forced")
#define __noreturn
#define container_of(ptr, type, member) \
        (type *)( (char *)(ptr) - offsetof(type,member) )
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#else

#warning Unsupported or untested compiler. Check compiler macros!

#define REORDER_BARRIER() ((void)0)
#define ACCESS_ONCE(x) (x)
#define UNUSED(x) __unused_##x
#define __always_inline inline
#define __noreturn
#define container_of(ptr, type, member) \
        (type *)( (char *)(ptr) - offsetof(type,member) )
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#endif

#endif /* COMPILER_H_ */
