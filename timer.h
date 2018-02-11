#ifndef __HAVE_TIMER_H__
#define __HAVE_TIMER_H__

#include <stdint.h>
#include <unistd.h>

#include "spectre_archs.h"
#include "spectre_intrinsics.h"

#if defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0 && defined(_POSIX_MONOTONIC_CLOCK)
#define __HAVE_POSIX_GETTIME__
#include <time.h>

uint64_t __posix_gettime(void) {
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	/* return value converted to nsec */
	return (uint64_t)((ts.tv_sec * 1000000000) + ts.tv_nsec);
}
#endif

#if defined(__MACH__)
#define __HAVE_MACH_GETTIME__
#include <mach/clock.h>
#include <mach/mach.h>

clock_serv_t __mach_cclock = 0;
uint64_t __mach_gettime(void) {
	mach_timespec_t mts;

	if (!__mach_cclock) {
		host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &__mach_cclock);
	}

	clock_get_time(__mach_cclock, &mts);
	//mach_port_deallocate(mach_task_self(), cclock);

	/* return value converted to nsec */
	return (uint64_t)((mts.tv_sec * 1000000000) + mts.tv_nsec);
}
#endif


/* define __gettime macro for platform-specific implementation */

#if defined(__ARCH_POWERPC__)
#define /* uint64_t */ __gettime(void)	__get_mftb()
#elif defined(__ARCH_S390X__)
#define /* uint64_t */ __gettime(void)	__tstck()
#elif defined(__ARCH_X86__)
#define /* uint64_t */ __gettime(void)	__get_rdtsc()
#elif defined(__HAVE_POSIX_GETTIME__)
#define /* uint64_t */ __gettime(void)	__posix_gettime()
#elif defined(__HAVE_MACH_GETTIME__)
#define /* uint64_t */ __gettime(void)	__mach_gettime()
#else
#error Cannot get time!
#endif

#endif
