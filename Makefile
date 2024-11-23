EXTRA_CFLAGS =
CFLAGS = -g -Wall -O3 $(EXTRA_CFLAGS)
LDFLAGS = -g -Wall -O3 $(EXTRA_CFLAGS)

all: demo1 demo2

demo1 demo2:: LDFLAGS += -lm
demo1: demo1.o eytzinger.o demo_print.o
demo2: demo2.o eytzinger.o demo_print.o

clean:
	rm -f demo1 demo1.o demo2 demo2.o eytzinger.o demo_print.o
