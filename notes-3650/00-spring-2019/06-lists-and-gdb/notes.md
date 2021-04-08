---
layout: default
---

# Computer Systems

## HW Questions?

## GDB Intro

segfault.c

```
int
main(int argc, char* argv[])
{
  long* foo = (long*) 7;
  printf("%ld\n", *foo);
}
```

```
### -g flag to add debug symbols
$ gcc -g -o segfault segfault.c
$ gdb ./segfault
...
(gdb) run
*segfault*
(gdb) list
(gdb) p foo
(gdb) p argc
```

segfault.S

```
.global main
.data
hello: .string "hello"

.text
main:
  enter $0, $0
  mov $main, %rdi
  call puts
  ret
```

```
$ gcc -no-pie -g -o segv segfault.S
$ gdb ./segv
(gdb) run
*segfault*
(gdb) list
(gdb) p $rdi
(gdb) p (char*) $rdi
```

 - recur.c
 
```
long
foo(char* text, long xx)
{
    if (xx < 0) {
        return 0;
    }
    else {
        return text[xx] + foo(text, 2*xx);
    }
}

int
main(int argc, char* argv[])
{
    printf("%ld\n", foo("hi", 5));
    return 0;
}
```

```
$ gdb ./recur
(gdb) run
*segfault*
(gdb) bt
(gdb) break foo
(gdb) run
(gdb) next # skip calls
(gdb) step # next line of code
```

## Linked Lists

Linked lists are the standard data structure from Fundies 1. A sequence of items
is built up from cells, which are a pair of {an item, a next pointer}.

They allow simple dynamic growth of the sequence, but don't allow constant
time random access like arrays.

 - Draw linked list on board.
 - Show cons.
 - Show reverse list program.

## Variable Length Arrays (Vectors)

Variable Length Arrays (vector in C++; ArrayList in Java).

Stores a sequence of items contiguously in memory. Resizes by copying to a larger
array when needed. Over-allocates the new array to prevent having to resize on
every insert.

 - Draw vec on board.
 - Show push, push, push
 - Show reverse vec program.
 
## Overflow

 - Translate rev-str-array.c

