
SRCS := $(wildcard *.c)
BINS := $(SRCS:.c=)

all: $(BINS)

% : %.c

clean:
	rm -f *.o $(BINS)

.PHONY: clean
