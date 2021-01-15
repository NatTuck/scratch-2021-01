#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long
sqrt_up(long xx)
{
    return (long)ceil(sqrt(xx));
}

int
is_prime(long primes[], long xx)
{
    for (long ii = 0; primes[ii] <= sqrt_up(xx); ++ii) {
        long pp = primes[ii];
        if ((xx % pp) == 0) {
            return 0;
        }
    }
    return 1;
}

long
prime(long nn) 
{
    long primes[nn];
    for (long ii = 0; ii < nn; ++ii) { 
        primes[ii] = 0;
    }
    
    primes[0] = 2;
    primes[1] = 3;
    primes[2] = 5;

    for (long ii = 3; ii < nn; ++ii) {
        long guess = primes[ii - 1] + 1;
        
        while (!is_prime(primes, guess)) {
            guess++;
        }

        primes[ii] = guess;
    }

    return primes[nn - 1];
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage:\n\t./prime [N]\n");
        return 1;
    }

    long nn = atol(argv[1]);
    long pp = prime(nn);

    printf("Prime %ld is %ld\n", nn, pp);

    return 0;
}
