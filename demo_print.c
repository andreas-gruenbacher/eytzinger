#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "eytzinger.h"
#include "demo_print.h"

void print(struct eytzinger *ez, bool header)
{
	int digits = ez->size == 0 ? 0 : floor(log10(ez->size)) + 1;
	unsigned int m;

	if (header) {
		printf("%*s ", digits, "");
		for (m = 0; m < ez->size; m++)
		    printf(" %0*u", digits, m);
		printf("\n");
	}

	printf("%*u:", digits, ez->size);
	for (m = 1; m <= ez->size; m++)
	    printf(" %0*u", digits, ez->array[m]);
	printf("\n");
}
