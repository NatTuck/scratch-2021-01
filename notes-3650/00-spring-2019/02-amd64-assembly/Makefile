
GAS := gcc -no-pie -g

all: ifact rfact

ifact: ifact.S
	$(GAS) -o ifact ifact.S

rfact: rfact.S
	$(GAS) -o rfact rfact.S

clean:
	rm -f ifact rfact

.PHONY: all clean
