EXTRA_CFLAGS =
CFLAGS = -g -Wall -O3 $(EXTRA_CFLAGS)
LDFLAGS = -g -Wall -O3 $(EXTRA_CFLAGS) -lm

all: test_eq test_iterators test_lt test_le test_ge test_gt

test_iterators test_eq test_lt test_le test_ge test_gt: test_common.o eytzinger.o

clean:
	rm -f test_eq test_eq.o test_iterators test_iterators.o test_lt test_lt.o test_le test_le.o test_ge test_ge.o test_gt test_gt.o eytzinger.o test_common.o
