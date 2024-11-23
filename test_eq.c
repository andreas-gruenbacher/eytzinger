#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "eytzinger.h"
#include "test_common.h"

typedef int key_t;

int main(int argc, char *argv[])
{
	struct eytzinger ez = {
		.cmp = int_cmp,
	};
	unsigned int n, nr;
	key_t *array;
	int i;

	assert(argc > 1);
	nr = atoi(argv[1]);
	array = malloc(nr * sizeof(key_t));
	for (n = 0; n < nr; n++)
		array[n] = n;
	ez_create(&ez, array, nr, sizeof(key_t));
	free(array);
	print(&ez, true);

	for (i = 2; i < argc; i++) {
		key_t key;
		ez_pos pos;

		key = atoi(argv[i]);
#if 0
		pos = ez_search_any_eq(&ez, &key);
#else
		pos = ez_search_first_eq(&ez, &key);
		if (pos) {
			key_t prev = ez_prev(pos, ez.nr);
			assert(!prev || ((int *)ez.array)[prev] < key);
		}
#endif
		printf("key %i %s at position %u\n",
		       key, pos ? "found" : "not found", pos);
	}
	ez_free(&ez);
	return 0;
}
