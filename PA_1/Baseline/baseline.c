#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    long double sum = 0.0;

    unsigned long long int n;
    printf("Enter N: ");
    scanf("%lld", &n);

    time_t start, end;
    start = time(NULL); // Start Timer
    for (unsigned long long int i = 0; i < n; i++)
        sum += i;
    end = time(NULL);                             // End Timer
    double elapsed_time_s = difftime(end, start); // Time difference in seconds
    printf("Sum: %Lf, Ellapsed Time: %f seconds\n", sum, elapsed_time_s);
    return 0;
}
