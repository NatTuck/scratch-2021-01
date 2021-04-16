SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CC := gcc
CFLAGS := -g

fib: $(OBJS)
	gcc $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -rf fib $(OBJS)
