#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int stack[5];
int stptr = 0;

void
stack_push(int xx)
{
    stack[stptr++] = xx;
}

int
stack_pop()
{
    return stack[stptr--];
}

int
main(int _ac, char* _av[])
{
    for (int ii = 0; ii < 5; ++ii) {
        stack_push(ii);
    }

    for (int ii = 0; ii < 5; ++ii) {
        int yy = stack_pop();
        printf("%d\n", yy);
    }

    return 0;
}
