#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Structure to hold thread partial sum data
 */
typedef struct
{
    unsigned long long int range_start;
    unsigned long long int range_end;
    long double partial_sum;
} PartialSumData;

int main(int argc, char const *argv[])
{
    unsigned long int n;
    unsigned int tasks_size;
    pid_t pids;

    printf("Enter N: ");
    scanf("%ld", &n);
    printf("Enter task size: ");
    scanf("%d", &tasks_size);

    PartialSumData task_data[tasks_size];
    unsigned long int range = n / tasks_size;
    for (int i = 0; i < tasks_size; i++)
    {
        task_data[i].range_start = i * range;
        task_data[i].range_end = i + 1 * range;
        task_data[i].partial_sum = 0;
        pids = fork();

        if (pids == 0)
        {
        }
    }

    // You can then determine uniquely identify the all available processes as a combination of pids array
}
