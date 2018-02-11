#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "spectre_archs.h"
#include "spectre_intrinsics.h"
#include "timer.h"

#define NUM_ROUNDS 4
#define CACHE_LINE_LEN 16

int main(void) {
	unsigned int i, j;
	uint32_t *values[CACHE_LINE_LEN];
	register uint64_t time1, time2;
	volatile uint32_t *value_ptr;

	/* fill values with pointers */
	for (i = 0; i < (CACHE_LINE_LEN - 1); ++i) {
		values[i] = (uint32_t *)&(values[i+1]);
	}
	values[(CACHE_LINE_LEN - 1)] = (uint32_t *)&(values[0]);

	uint64_t sum = 0;

	for (i = 0; i < NUM_ROUNDS; ++i) {
		value_ptr = (uint32_t *)&values;
		for (j = 0; j < CACHE_LINE_LEN; ++j) {
			/* read timer (start) */
			time1 = __gettime();

			/* memory access to time */
			value_ptr = (uint32_t *)(*value_ptr);

			/* read timer (end) */
			time2 = __gettime();

			/* print duration */
			unsigned int duration = (time2 - time1);
			sum += duration;
			printf("%3u,%3u: %u\n", i, j, duration);
		}
	}

	printf("\nAvg: %.2f\n", (double)(sum / NUM_ROUNDS / CACHE_LINE_LEN));

	return EXIT_SUCCESS;
}
