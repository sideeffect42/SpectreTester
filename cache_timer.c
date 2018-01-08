#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "spectre_archs.h"
#include "spectre_intrinsics.h"
#include "timer.h"

#define NUM_ROUNDS 32

unsigned int _value = 1; /* just some value */

int main(void) {
	unsigned int i, junk;
	register uint64_t time1, time2;
	volatile unsigned int *_value_addr;

	_value_addr = &_value;

	/* preload value */
	junk = *_value_addr;

	uint64_t sum = 0;

	for (i = 0; i < NUM_ROUNDS; ++i) {
		/* read timer (start) */
		time1 = __gettime();

		/* memory access to time */
		junk = *_value_addr; 

		/* read timer (end) */
		time2 = __gettime();

		/* print duration */
		unsigned int duration = (time2 - time1);
		sum += duration;
		printf("%3u: %u\n", i, duration);
	}

	printf("\nAvg: %.2f\n", (double)(sum / NUM_ROUNDS));

	return EXIT_SUCCESS;
}
