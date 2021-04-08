# This should be named Makefile

SRCS := $(wildcard *.c)
BINS := $(SRCS:.c=)

CC := gcc
CFLAGS := -g -std=gnu11 -O0
LDLIBS := -lpthread

all: $(BINS)

clean:
	rm -f $(BINS)

.PHONY: all clean

