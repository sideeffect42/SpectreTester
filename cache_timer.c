#include <stdio.h>

#include "cache.h"

int main(void) {
	int avg;
	avg = determine_cache_latency();
	printf("\nAvg: %u\n", avg);

	return EXIT_SUCCESS;
}
