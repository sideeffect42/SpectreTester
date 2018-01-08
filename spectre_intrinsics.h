#include "spectre_archs.h"

#ifndef __SPECTRE_INTRINSICS__
#define __SPECTRE_INTRINSICS__

#if defined(__ARCH_POWERPC__)

#if defined(__APPLE__)
#include <ppc_intrinsics.h>
#else

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
