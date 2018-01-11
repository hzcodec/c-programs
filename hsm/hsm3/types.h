//============================================================================
//
// $URL: svn+ssh://ristna.unjo.local/var/svn/220_06_005_COMMON/trunk/include/types.h $
// $Revision: 6762 $
// $Date: 2013-04-25 18:56:13 +0200 (tor, 25 apr 2013) $
// $Author: andreas.fritiofson $
//
// Description :
//
// Copyright   : UNJO AB
//
//============================================================================

#ifndef TYPES_H_
#define TYPES_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "compiler.h"

#define COUNTOF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#endif /* TYPES_H_ */
