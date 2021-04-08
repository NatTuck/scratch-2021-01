

all: save-array print-array

save-array: save-array.c
	gcc -g -o save-array save-array.c

print-array: print-array.c
	gcc -g -o print-array print-array.c

clean:
	rm -f save-array print-array array.mem

.PHONY: all clean
