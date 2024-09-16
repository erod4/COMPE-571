#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    unsigned long long int sum = 0;
    if (argc < 2)
    {
        printf("you must provide a value for n in order to calculate values from 0 to n\n");
        return 1;
    }
    /**
     * atoi function is used to convert a numeric string into an integer value.
     * https://www.tutorialspoint.com/c_standard_library/c_function_atoi.htm
     */
    int n = atoi(argv[1]);

    clock_t start, end;
    start = clock();
    for (unsigned long long int i = 0; i < n; i++)
        sum += i;
    end = clock();
    double elapsed_time_ms = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;

    printf("Sum: %lld, Ellapsed Time: %f ms\n", sum, elapsed_time_ms);
    return 0;
}
