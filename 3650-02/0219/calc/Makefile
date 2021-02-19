
# This is GNU Make, it's worth reading the manual

HDRS := $(wildcard *.h)

calc: ast.o  calc.o  list.o  parse.o  tokenize.o
	gcc -g -o calc ast.o  calc.o  list.o  parse.o  tokenize.o

ast.o: ast.c $(HDRS)
	gcc -c -g -o ast.o ast.c

calc.o: calc.c $(HDRS)
	gcc -c -g -o calc.o calc.c

list.o: list.c $(HDRS)
	gcc -c -g -o list.o list.c

parse.o: parse.c $(HDRS)
	gcc -c -g -o parse.o parse.c

tokenize.o: tokenize.c $(HDRS)
	gcc -c -g -o tokenize.o tokenize.c

clean:
	rm -f calc *.o 

check:
	clang-check *.c --
