
all: prime prime2

prime2: prime2.c
	gcc -o prime2 prime2.c -lm

prime: prime.c
	gcc -o prime prime.c -lm

clean:
	rm -f prime prime2

.PHONY: all clean
