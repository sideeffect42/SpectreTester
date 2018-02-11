#ifndef __HAVE_CACHE_LATENCY_H__
#define __HAVE_CACHE_LATENCY_H__

#include "spectre_archs.h"
#include "spectre_intrinsics.h"
#include "timer.h"

#define NUM_ROUNDS 1
#define CACHE_LINE_LEN 8

unsigned int determine_cache_latency() {
	unsigned int i, j;
	register uint64_t time1, time2;
	void **values[CACHE_LINE_LEN];
	volatile void **value_ptr;

	/* fill values with pointers */
	for (i = 0; i < (CACHE_LINE_LEN - 1); i++) {
		values[i] = (void **)&(values[i+1]);
	}
	values[(CACHE_LINE_LEN - 1)] = (void **)&(values[0]);

	uint64_t sum = 0;

	for (i = 0; i < NUM_ROUNDS; i++) {
		value_ptr = values[0];
		for (j = 0; j < CACHE_LINE_LEN; j++) {
			/* read timer (start) */
			time1 = __gettime();

			/* memory access to time */
			value_ptr = (void **)(*value_ptr);

			/* read timer (end) */
			time2 = __gettime();

			/* print duration */
			unsigned int duration = (time2 - time1);
			sum += duration;
			printf("%3u,%3u: %u\n", i, j, duration);
		}

		/* make a little pause after round */
		sleep(1);
	}

	return ((double)sum / (NUM_ROUNDS * CACHE_LINE_LEN));
}

#endif
