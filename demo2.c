#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "eytzinger.h"
#include "demo_print.h"

int main(int argc, char *argv[])
{
	struct eytzinger ez;
	unsigned int m, size;
	ez_key *array;
	ez_key k;

	assert(argc > 1);
	size = atoi(argv[1]);
	assert(size >= 1);
	array = malloc(size * sizeof(ez_key));
	for (m = 0; m < size; m++)
		array[m] = m;

	ez_create(&ez, array, size);
	free(array);
	print(&ez, true);

	m = 0;
	k = ez_first(size);
	do {
		assert(k <= size);
		assert(ez.array[k] == m++);
		k = ez_next(k, size);
	} while (k);
	assert(m == size);

	m = size;
	k = ez_last(size);
	do {
		assert(k <= size);
		assert(ez.array[k] == --m);
		k = ez_previous(k, size);
	} while (k);
	assert(m == 0);

	ez_free(&ez);
	return 0;
}
