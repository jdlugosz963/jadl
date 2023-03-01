#include "memory.h"

#include <stdio.h>

void *jadl_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr == NULL)
	{
		printf("malloc: ERROR");
		exit(1);
	}
	return ptr;
}

void jadl_free(void *ptr) {
	return free(ptr);
}

long usage() {
	struct rusage r_usage;
	getrusage(RUSAGE_SELF, &r_usage);
	return r_usage.ru_maxrss;
}

void usage_print() {
	printf("Mem usage: %ld kilobytes\n", usage());
}
