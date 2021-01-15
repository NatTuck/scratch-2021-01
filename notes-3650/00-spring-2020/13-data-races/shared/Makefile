
SRCS := $(wildcard *.c)
BINS := $(SRCS:.c=)

CC := gcc
CFLAGS := -g -O0 --std=gnu11
LDLIBS := -lpthread

all: $(BINS)

clean:
	rm -f $(BINS)

.PHONY: all clean

