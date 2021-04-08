
CFLAGS := -g -Wall -Werror
SRCS   := $(wildcard *.c)

shared: $(SRCS)
	gcc $(CFLAGS) -o shared  $(SRCS) -lm -lpthread

clean:
	rm -f shared

.PHONY: clean

