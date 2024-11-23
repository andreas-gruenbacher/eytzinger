#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "eytzinger.h"
#include "test_common.h"

int main(int argc, char *argv[])
{
	struct eytzinger ez = {
		.cmp = int_cmp,
	};
	unsigned int n, nr;
	int *array;
	ez_pos pos;

	assert(argc > 1);
	nr = atoi(argv[1]);
	assert(nr >= 1);
	array = malloc(nr * sizeof(int));
	for (n = 0; n < nr; n++)
		array[n] = n;

	ez_create(&ez, array, nr, sizeof(int));
	free(array);
	print(&ez, true);

	n = 0;
	pos = ez_first(ez.nr);
	do {
		assert(pos >= 1 && pos <= ez.nr);
		assert(((int *)ez.array)[pos] == n++);
		pos = ez_next(pos, ez.nr);
	} while (pos);
	assert(n == ez.nr);

	n = ez.nr;
	pos = ez_last(ez.nr);
	do {
		assert(pos >= 1 && pos <= ez.nr);
		assert(((int *)ez.array)[pos] == --n);
		pos = ez_prev(pos, ez.nr);
	} while (pos);
	assert(n == 0);

	ez_free(&ez);
	return 0;
}
