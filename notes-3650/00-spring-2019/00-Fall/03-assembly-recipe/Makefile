
GAS := gcc -no-pie -g

all: ifact rfact strlen

ifact: ifact.S
	$(GAS) -o ifact ifact.S

rfact: rfact.S
	$(GAS) -o rfact rfact.S

strlen: strlen.o readline.o
	$(GAS) -o strlen strlen.o readline.o

strlen.o: strlen.S
	$(GAS) -c -o strlen.o strlen.S

readline.o: readline.c
	$(GAS) -c -o readline.o readline.c

clean:
	rm -f ifact rfact strlen *.o

.PHONY: all clean
