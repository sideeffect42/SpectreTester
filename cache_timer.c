#include <stdio.h>

#include "cache_latency.h"

int main(void) {
	int avg;
	avg = determine_cache_latency();
	printf("\nAvg: %u\n", avg);

	return EXIT_SUCCESS;
}
