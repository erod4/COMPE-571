#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    unsigned long int n;
    int tasks_size;
    long double sum = 0.0;
    int fd[2];
    time_t start_t, end_t;

    // get input from user
    printf("Enter N: \n");
    scanf("%ld", &n);
    printf("Enter task size: \n");
    scanf("%d", &tasks_size);

    // calculate range size for each task
    unsigned long int range = n / tasks_size;
    pid_t pids[tasks_size];
    // create a pipe for tasks to communicate
    pipe(fd);

    start_t = time(NULL); // Start Timer
    for (int i = 0; i < tasks_size; i++)
    {
        pids[i] = fork(); //> create a fork for m task size

        if (pids[i] == 0) //> check if its a child task
        {

            // calcuate ith task start, end, and init its partial sum
            long int start = i * range;
            long int end = (i + 1) * range;
            long double partial_sum = 0.0;

            for (unsigned long int j = start; j < end; j++) //> perform summation for ith task
            {
                partial_sum += j;
            }

            close(fd[0]);                                    //> close read size of pipe
            write(fd[1], &partial_sum, sizeof(partial_sum)); //> write ith tasks partial sum to the pipe
            exit(0);                                         //> terminate child task to prevent child task from creating a new task
        }
    }

    for (int i = 0; i < tasks_size; i++)
    {
        waitpid(pids[i], NULL, 0); //> parent task wait for all child tasks to complete
    }

    // loop through and collect partial sums from  child tasks
    for (int i = 0; i < tasks_size; i++)
    {
        long double partial_sum = 0.0;
        close(fd[1]);                                   //> close write side of pipe
        read(fd[0], &partial_sum, sizeof(partial_sum)); //> read from pipe and store into partial_sum temp var
        sum += partial_sum;                             //> increment sum with each childs partial sum
    }

    end_t = time(NULL);                               //> End Timer
    double elapsed_time_s = difftime(end_t, start_t); //> Time difference in seconds
    printf("Sum: %Lf, Ellapsed Time: %f seconds\n", sum, elapsed_time_s);
}
