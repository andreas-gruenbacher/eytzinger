#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "eytzinger.h"
#include "bitops.h"

static inline bool ez_beyond_end(ez_pos pos, unsigned int size)
{
	return pos > size;
}

static inline ez_pos ez_left_child(ez_pos pos)
{
	return pos << 1;
}

static inline ez_pos ez_right_child(ez_pos pos)
{
	return (pos << 1) + 1;
}

static inline ez_pos ez_parent(ez_pos pos)
{
	return pos >> 1;
}

static inline bool ez_is_right_child(ez_pos pos)
{
	return pos & 1;
}

static inline bool ez_is_left_child(ez_pos pos)
{
	return !ez_is_right_child(pos);
}

ez_pos ez_first(unsigned int size)
{
	if (size == 0)
		return 0;
	return 1 << log2(size);
}

ez_pos ez_last(unsigned int size)
{
	if (size == 0)
		return 0;
	return (1 << log2(size + 1)) - 1;
}

ez_pos ez_next(ez_pos pos, unsigned int size)
{
#if SLOW
	pos = ez_right_child(pos);
	if (ez_beyond_end(pos, size)) {
		pos = ez_parent(pos);
		while (ez_is_right_child(pos))
			pos = ez_parent(pos);
		return ez_parent(pos);
	} else {
		do {
			pos = ez_left_child(pos);
		} while (!ez_beyond_end(pos, size));
		return ez_parent(pos);
	}
#else
	pos = ez_right_child(pos);
	if (ez_beyond_end(pos, size)) {
		pos >>= ffs(~pos);
	} else {
		pos <<= clz(pos) - clz(size);
		pos >>= ez_beyond_end(pos, size);
	}
	return pos;
#endif
}

ez_pos ez_previous(ez_pos pos, unsigned int size)
{
#ifdef SLOW
	pos = ez_left_child(pos);
	if (ez_beyond_end(pos, size)) {
		pos = ez_parent(pos);
		while (ez_is_left_child(pos))
			pos = ez_parent(pos);
		return ez_parent(pos);
	} else {
		do {
			pos = ez_right_child(pos);
		} while (!ez_beyond_end(pos, size));
		return ez_parent(pos);
	}
#else
	pos = ez_left_child(pos);
	if (ez_beyond_end(pos, size)) {
		pos >>= ffs(pos);
	} else {
		pos = ((pos + 1) << (clz(pos) - clz(size))) - 1;
		pos >>= ez_beyond_end(pos, size);
	}
	return pos;
#endif
}

static void ez_alloc(struct eytzinger *ez, unsigned int size)
{
	ez_key *array;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
	array = malloc(size * sizeof(ez_key));
#pragma GCC diagnostic pop
	assert(array);
	ez->array = array - 1;
	ez->size = size;
}

static unsigned int ez_create_rec(struct eytzinger *ez, ez_key array[],
				  unsigned int i, ez_pos pos)
{
	if (pos <= ez->size) {
		i = ez_create_rec(ez, array, i, ez_left_child(pos));
		ez->array[pos] = array[i++];
		i = ez_create_rec(ez, array, i, ez_right_child(pos));
	}
	return i;
}

void ez_create(struct eytzinger *ez, ez_key array[], unsigned int size)
{
	ez_alloc(ez, size);
	ez_create_rec(ez, array, 0, 1);
}

void ez_free(struct eytzinger *ez)
{
	ez_key *array = ez->array + 1;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
	free(array);
#pragma GCC diagnostic pop
}

ez_pos ez_search(struct eytzinger *ez, ez_key key)
{
	ez_pos pos = 1;

	while (!ez_beyond_end(pos, ez->size)) {
		pos = (pos << 1) + (ez->array[pos] < key);
	}
	pos >>= ffs(~pos);
	return pos;
}
