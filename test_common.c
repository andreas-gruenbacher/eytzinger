#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "eytzinger.h"
#include "test_common.h"

int int_cmp(const void *a, const void *b)
{
	int _a = *(const int *)a;
	int _b = *(const int *)b;

	return (_a > _b) - (_a < _b);
}

void print(struct eytzinger *ez, bool header)
{
	int digits = ez->nr == 0 ? 0 : floor(log10(ez->nr)) + 1;
	unsigned int n;

	if (header) {
		printf("%*s ", digits, "");
		for (n = 1; n <= ez->nr; n++)
		    printf(" %0*u", digits, n);
		printf("\n");
	}

	printf("%*u:", digits, ez->nr);
	for (n = 1; n <= ez->nr; n++)
	    printf(" %0*u", digits, ((int *)ez->array)[n]);
	printf("\n");
}
