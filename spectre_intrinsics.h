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

#define __get_mftb() ((uint64_t)__mftb() | ((uint64_t)__mftbu() << 32))

#elif defined(__ARCH_ARM__)
// TODO

#elif defined(__ARCH_S390X__)
// FIXME: Untested
#if defined(__GNUC__)
#include <s390intrin.h>
#endif

static inline utin64_t __tstck() {
	uint64_t tsc;
	asm("\tstck\t%0\n" : "=Q" (tsc) : : "cc");
	return tsc;
}

#elif defined(__ARCH_X86__)

/* Load intrinsic headers for rdtscp and clflush */
#ifdef _MSC_VER
#include <intrin.h>
#pragma optimize("gt",on)
#elif defined(__GNUC__)
#include <x86intrin.h>
#endif

unsigned int __x86_rdtscp_temp; /* unused */
#define __get_rdtscp() ((uint64_t)__rdtscp(&__x86_rdtscp_temp))

static int __x86_rdtsc_call_num = 0;
static inline uint64_t __get_rdtsc() {
	register uint32_t cycles_high, cycles_low;

	/* because we need the cpuid call to serialize rdtsc we want to make sure
	 * that the time of cpuid is not included in the measured time */
	if (((++__x86_rdtsc_call_num) % 2)) {
		__asm__ volatile(
			"cpuid\n\t"
			"rdtsc\n\t"
			"mov %%edx, %0\n\t"
			"mov %%eax, %1\n\t"
			: "=r"(cycles_high), "=r"(cycles_low)
			:: "%rax", "%rbx", "%rcx", "%rdx"
		);
	} else {
		__asm__ volatile(
			"rdtsc\n\t"
			"mov %%edx, %0\n\t"
			"mov %%eax, %1\n\t"
			"cpuid\n\t"
			: "=r"(cycles_high), "=r"(cycles_low)
			:: "%rax", "%rbx", "%rcx", "%rdx"
		);
	}

	return (((uint64_t)cycles_high << 32) + cycles_low);
}

#endif
#endif
