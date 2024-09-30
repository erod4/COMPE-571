#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef struct
{
    unsigned long long int range_start;
    unsigned long long int range_end;
    long double partial_sum;
} PartialSumData;

void *calc_sum(void *vargs);

int main(int argc, char const *argv[])
{

    unsigned long long int n;
    long double sum = 0.0;
    int num_threads;
    time_t start, end;

    printf("Enter N: \n");
    scanf("%lld", &n);
    printf("Enter number of threads: \n");
    scanf("%d", &num_threads);

    long long int range = n / num_threads;

    pthread_t thread_ids[num_threads]; //> Thread id array with index corresponding to thread
    PartialSumData thread_data[num_threads];

    start = time(NULL); //> Start Timer
    for (int i = 0; i < num_threads; i++)
    {
        thread_data[i].range_start = i * range;     //> thread 1 range start 0, thread 2 start (n/num_threads), etc
        thread_data[i].range_end = (i + 1) * range; //> thread 1 range end (n/num_threads) [not inclusive], thread 2 range end 2*(n/num_threads) [not inclusive],etc
        thread_data[i].partial_sum = 0.0;           //> individual threads start their sum at 0

        pthread_create(&thread_ids[i], NULL, calc_sum, (void *)&thread_data[i]); // Create threads for number of threads and passes respective thread_data object to thread
    }
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(thread_ids[i], NULL);
        sum += thread_data[i].partial_sum;
    }
    end = time(NULL); // End Timer

    double elapsed_time_s = difftime(end, start); //> Time difference in seconds
    printf("Sum: %Lf, Ellapsed Time: %f seconds\n", sum, elapsed_time_s);

    return 0;
}
void *calc_sum(void *thread_data)
{
    PartialSumData *data = (PartialSumData *)thread_data;

    for (unsigned long long int i = data->range_start; i < data->range_end; i++)
    {
        data->partial_sum += i;
    }
    pthread_exit(NULL);
}
