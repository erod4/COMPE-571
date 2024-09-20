#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
int main(int argc, char const *argv[])
{
    unsigned long int n;
    int tasks_size;
    long double sum = 0.0;
    int fd[2];

    printf("Enter N: ");
    scanf("%ld", &n);
    printf("Enter task size: ");
    scanf("%d", &tasks_size);

    unsigned long int range = n / tasks_size;
    pid_t pids[tasks_size];

    pipe(fd);

    for (int i = 0; i < tasks_size; i++)
    {
        pids[i] = fork();

        if (pids[i] == 0)
        {
            long int start = i * range;
            long int end = (i + 1) * range;
            long double partial_sum = 0.0;

            for (unsigned long int j = start; j < end; j++)
            {
                partial_sum += j;
            }
            printf("Start %ld\n", start);
            printf("End %ld\n", end);
            printf("Child Partial Sum: %Lf\n", partial_sum);
            close(fd[0]);
            write(fd[1], &partial_sum, sizeof(partial_sum));
            exit(0);
        }
        }

    for (int i = 0; i < tasks_size; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
    for (int i = 0; i < tasks_size; i++)
    {
        long double partial_sum = 0.0;
        close(fd[1]);
        read(fd[0], &partial_sum, sizeof(partial_sum));
        printf("Partial Sum Sent To Parent: %Lf\n", partial_sum);
        sum += partial_sum;
    }

    printf("Sum %Lf\n", sum);
}
