
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <alloca.h>

long
sqrt_up(long xx)
{
    return (long)ceil(sqrt(xx));
}

int
is_prime(long primes[], long xx)
{
    for (long ii = 0; primes[ii] <= sqrt_up(xx); ++ii) {
        long pr = primes[ii];
        if ((xx % pr) == 0) {
            return 0;
        }
    }
    return 1;
}

long
prime(long nn)
{
    long* primes = alloca(nn * sizeof(long));
    for (long ii = 0; ii < nn; ++ii) {
        primes[nn] = 0;
    }

    primes[0] = 2;
    primes[1] = 3;
    primes[2] = 5;

    for (long ii = 3; ii < nn; ++ii) {
        long guess = primes[ii-1] + 1;

        while (!is_prime(primes, guess)) {
            guess++;
        }

        // primes[ii]
        *(primes + ii) = guess;

        // primes[ii]
        // (primes,ii,8) => primes + 8*ii
        //
        // *(primes + ii)
        // (primes,ii,8) => primes + sizeof(primes[ii])*ii

        // ptr + int
        // ptr + int*sizeof(ptr)

        // &(primes[ii])
        // (primes + ii)
    }

    return primes[nn-1];
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        puts("Nope");
        return 1;
    }

    long nn = atol(argv[1]);
    // Calculate the first nn primes.
    // Print the last one.
    long pr = prime(nn);

    printf("Prime %ld = %ld\n", nn, pr);

    return 0;
}
