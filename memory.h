#ifndef _MEMORY
#define _MEMORY

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void *jadl_malloc(size_t size);
void jadl_free(void *ptr);

long usage();
void usage_print();

#endif
