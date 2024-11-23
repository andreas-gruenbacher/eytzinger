typedef unsigned int ez_pos;

struct eytzinger {
	void *array;
	unsigned int nr;
	unsigned int size;
	int (*cmp)(const void *, const void *);
};

void ez_create(struct eytzinger *ez, const void *array, unsigned int nr, unsigned int size);
void ez_free(struct eytzinger *ez);

ez_pos ez_search_any_eq(struct eytzinger *ez, void *key);
ez_pos ez_search_first_eq(struct eytzinger *ez, void *key);

ez_pos ez_search_lt(struct eytzinger *ez, void *key);
ez_pos ez_search_le(struct eytzinger *ez, void *key);
ez_pos ez_search_ge(struct eytzinger *ez, void *key);
ez_pos ez_search_gt(struct eytzinger *ez, void *key);

ez_pos ez_first(unsigned int nr);
ez_pos ez_last(unsigned int nr);
ez_pos ez_next(ez_pos k, unsigned int nr);
ez_pos ez_prev(ez_pos k, unsigned int nr);

static void *ez_key(struct eytzinger *ez, ez_pos pos)
{
	return ez->array + ez->size * pos;
}

static inline int ez_cmp(struct eytzinger *ez, ez_pos pos, const void *key)
{
	return ez->cmp(ez_key(ez, pos), key);
}
