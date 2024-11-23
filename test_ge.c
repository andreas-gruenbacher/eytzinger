#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "eytzinger.h"
#include "test_common.h"

typedef int key_t;

void verify_ge(struct eytzinger *ez, key_t key, ez_pos pos)
{
	if (!pos) {
		ez_pos last;

		last = ez_last(ez->nr);
		assert(!last || ez_cmp(ez, last, &key) < 0);
	} else {
		ez_pos prev;

		assert(ez_cmp(ez, pos, &key) >= 0);
		prev = ez_prev(pos, ez->nr);
		assert(!prev || ez_cmp(ez, prev, &key) < 0);
	}
}

int main(int argc, char *argv[])
{
	struct eytzinger ez = {
		.cmp = int_cmp,
	};
	unsigned int n, nr, maxsize;
	key_t *array;

	assert(argc > 1);
	maxsize = atoi(argv[1]);
	array = malloc(maxsize * sizeof(key_t));
	for (n = 0; n < maxsize; n++)
		array[n] = n + 1;

	for (nr = 1; nr <= maxsize; nr++) {
		ez_create(&ez, array, nr, sizeof(key_t));
		// print(&ez, true);
		for (n = 0; n < nr + 2; n++) {
			key_t key = n;
			ez_pos pos;

			pos = ez_search_ge(&ez, &key);
			// printf("ez_search_ge(&ez, %d) = %u\n", n, pos);
			verify_ge(&ez, n, pos);
		}
		ez_free(&ez);
	}
	free(array);
	return 0;
}
