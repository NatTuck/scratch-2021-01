#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

// .data
int stack[5];
int stptr = 0;  // where the next item goes
// There's proably other vars
// from libraries in .data

void
stack_push(int xx)
{
    stack[stptr++] = xx;
}

int
stack_pop()
{
    return stack[--stptr];
}

#define NN 1005

int
main(int _ac, char* _av[])
{
    for (int ii = 0; ii < NN; ++ii) {
        stack_push(ii);
    }

    for (int ii = 0; ii < NN; ++ii) {
        int yy = stack_pop();
        printf("%d\n", yy);
    }

    return 0;
}
