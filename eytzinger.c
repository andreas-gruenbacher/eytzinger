#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "eytzinger.h"
#include "bitops.h"

static inline bool ez_beyond_end(ez_pos pos, unsigned int nr)
{
	return pos > nr;
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

ez_pos ez_first(unsigned int nr)
{
	if (nr == 0)
		return 0;
	return 1 << log2(nr);
}

ez_pos ez_last(unsigned int nr)
{
	if (nr == 0)
		return 0;
	return (1 << log2(nr + 1)) - 1;
}

ez_pos ez_next(ez_pos pos, unsigned int nr)
{
#if SLOW
	pos = ez_right_child(pos);
	if (ez_beyond_end(pos, nr)) {
		pos = ez_parent(pos);
		while (ez_is_right_child(pos))
			pos = ez_parent(pos);
		return ez_parent(pos);
	} else {
		do {
			pos = ez_left_child(pos);
		} while (!ez_beyond_end(pos, nr));
		return ez_parent(pos);
	}
#else
	pos = ez_right_child(pos);
	if (ez_beyond_end(pos, nr)) {
		pos >>= ffs(~pos);
	} else {
		pos <<= clz(pos) - clz(nr);
		pos >>= ez_beyond_end(pos, nr);
	}
	return pos;
#endif
}

ez_pos ez_prev(ez_pos pos, unsigned int nr)
{
#ifdef SLOW
	pos = ez_left_child(pos);
	if (ez_beyond_end(pos, nr)) {
		pos = ez_parent(pos);
		while (ez_is_left_child(pos))
			pos = ez_parent(pos);
		return ez_parent(pos);
	} else {
		do {
			pos = ez_right_child(pos);
		} while (!ez_beyond_end(pos, nr));
		return ez_parent(pos);
	}
#else
	pos = ez_left_child(pos);
	if (ez_beyond_end(pos, nr)) {
		pos >>= ffs(pos);
	} else {
		pos = ((pos + 1) << (clz(pos) - clz(nr))) - 1;
		pos >>= ez_beyond_end(pos, nr);
	}
	return pos;
#endif
}

static void ez_alloc(struct eytzinger *ez, unsigned int nr, unsigned int size)
{
	void *array;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
	array = malloc(nr * size);
#pragma GCC diagnostic pop
	assert(array);
	ez->array = array - size;
	ez->nr = nr;
	ez->size = size;
}

static unsigned int ez_create_rec(struct eytzinger *ez, const void *array,
				  unsigned int i, ez_pos pos)
{
	unsigned int size = ez->size;

	if (pos <= ez->nr) {
		i = ez_create_rec(ez, array, i, ez_left_child(pos));
		memcpy(ez->array + size * pos, array + size * i, size);
		i++;
		i = ez_create_rec(ez, array, i, ez_right_child(pos));
	}
	return i;
}

void ez_create(struct eytzinger *ez, const void *array, unsigned int nr, unsigned int size)
{
	ez_alloc(ez, nr, size);
	ez_create_rec(ez, array, 0, 1);
}

void ez_free(struct eytzinger *ez)
{
	void *array = ez->array + ez->size;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
	free(array);
#pragma GCC diagnostic pop
}

ez_pos ez_search_first_eq(struct eytzinger *ez, void *key)
{
	ez_pos pos = 1;

	while (!ez_beyond_end(pos, ez->nr))
		pos = (pos << 1) + (ez_cmp(ez, pos, key) < 0);
	pos >>= ffs(pos + 1);
	if (pos && ez_cmp(ez, pos, key))
		pos = 0;
	return pos;
}

ez_pos ez_search_any_eq(struct eytzinger *ez, void *key)
{
	ez_pos pos = 1;

	while (!ez_beyond_end(pos, ez->nr)) {
		int cmp = ez_cmp(ez, pos, key);
		if (!cmp)
			return pos;
		pos = (pos << 1) + (cmp < 0);
	}
	return 0;
}

ez_pos ez_search_lt(struct eytzinger *ez, void *key)
{
	ez_pos pos = 1;

	while (!ez_beyond_end(pos, ez->nr))
		pos = (pos << 1) + (ez_cmp(ez, pos, key) < 0);
	pos >>= ffs(pos);
	return pos;
}

ez_pos ez_search_le(struct eytzinger *ez, void *key)
{
	ez_pos pos = 1;

	while (!ez_beyond_end(pos, ez->nr))
		pos = (pos << 1) + (ez_cmp(ez, pos, key) <= 0);
	pos >>= ffs(pos);
	return pos;
}

ez_pos ez_search_ge(struct eytzinger *ez, void *key)
{
	ez_pos pos = 1;

	while (!ez_beyond_end(pos, ez->nr))
		pos = (pos << 1) + (ez_cmp(ez, pos, key) < 0);
	pos >>= ffs(pos + 1);
	return pos;
}

ez_pos ez_search_gt(struct eytzinger *ez, void *key)
{
	ez_pos pos = 1;

	while (!ez_beyond_end(pos, ez->nr))
		pos = (pos << 1) + (ez_cmp(ez, pos, key) <= 0);
	pos >>= ffs(pos + 1);
	return pos;
}
