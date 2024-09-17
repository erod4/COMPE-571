#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    long double sum = 0.0;
    if (argc < 2)
    {
        printf("you must provide a value for n in order to calculate values from 0 to n\n");
        return 1;
    }
    /**
     * strtoull function is used to convert a numeric string into an integer value.
     * https://cplusplus.com/reference/cstdlib/strtoull/
     */
    unsigned long long int n = strtoull(argv[1], NULL, 10);

    time_t start, end;
    printf("N: %lld\n", n);
    start = time(NULL); // Start Timer
    for (unsigned long long int i = 0; i < n; i++)
        sum += i;
    end = time(NULL);                             // End Timer
    double elapsed_time_s = difftime(end, start); // Time difference in seconds
    printf("Sum: %Lf, Ellapsed Time: %f seconds\n", sum, elapsed_time_s);
    return 0;
}
