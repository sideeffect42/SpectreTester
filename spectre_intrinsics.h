#include "spectre_archs.h"

#ifndef __SPECTRE_INTRINSICS__
#define __SPECTRE_INTRINSICS__

#if defined(__ARCH_POWERPC__)

#if defined(__APPLE__)
#include <ppc_intrinsics.h>
#else
#define __SPR_TBL       268             /* Time-base Lower. Not on PPC 601 */
#define __SPR_TBU       269             /* Time-base Upper. Not on PPC 601 */

#define __mfspr(spr)    \
  __extension__ ({ long mfsprResult; \
     __asm__ volatile ("mfspr %0, %1" : "=r" (mfsprResult) : "n" (spr)); \
     /*return*/ mfsprResult; })

#define __dcbf(base, index)     \
  __asm__ ("dcbf %0, %1" : /*no result*/ : "b%" (index), "r" (base) : "memory")
#define __mftb()                __mfspr(__SPR_TBL)
#define __mftbu()               __mfspr(__SPR_TBU)
#endif

#elif defined(__ARCH_ARM__)
// TODO

#elif defined(__ARCH_X86__)

/* Load intrinsic headers for rdtscp and clflush */
#ifdef _MSC_VER
#include <intrin.h>
#pragma optimize("gt",on)
#else
#include <x86intrin.h>
#endif
#endif
#endif
