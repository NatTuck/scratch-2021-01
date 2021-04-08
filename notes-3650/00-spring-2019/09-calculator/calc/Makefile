
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
HDRS := $(wildcard *.h)

calc: $(OBJS)
	gcc -g -o calc $(SRCS) $(HDRS)

$(OBJS): $(SRCS) $(HDRS)
	gcc -c -g -o $@ $<

clean:
	rm -f calc *.o 

check:
	clang-check *.c --
