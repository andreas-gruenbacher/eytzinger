typedef unsigned int ez_pos;
typedef int ez_key;

struct eytzinger {
	ez_key *array;
	unsigned int size;
};

void ez_create(struct eytzinger *ez, ez_key array[], unsigned int size);
void ez_free(struct eytzinger *ez);

ez_pos ez_search(struct eytzinger *ez, ez_key key);

ez_pos ez_first(unsigned int size);
ez_pos ez_last(unsigned int size);
ez_pos ez_next(ez_pos k, unsigned int size);
ez_pos ez_previous(ez_pos k, unsigned int size);
