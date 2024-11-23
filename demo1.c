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
	int i;

	assert(argc > 1);
	size = atoi(argv[1]);
	array = malloc(size * sizeof(ez_key));
	for (m = 0; m < size; m++)
		array[m] = m;
	ez_create(&ez, array, size);
	free(array);
	print(&ez, true);

	for (i = 2; i < argc; i++) {
		ez_key key;
		ez_pos pos;
		bool found;

		key = atoi(argv[i]);
		pos = ez_search(&ez, key);
		found = pos && ez.array[pos] == key;
		printf("key %i %s at position %u\n",
		       key, found ? "found" : "not found", pos);
			break;
	}
	ez_free(&ez);
	return 0;
}
