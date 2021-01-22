long
add1(long xx)
{
    return xx + 2;
}

int
main(int _ac, char* _av[])
{
    long y = add1(5);
    printf("%ld\n", y);
    return 0;
}
