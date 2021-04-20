SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CC := gcc
CFLAGS := -g

fib: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -rf fib $(OBJS)

.PHONY: clean
