#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <stdint.h>

int main()
{

    struct timespec arrival_time, end_time, delayT;

    clock_gettime(CLOCK_MONOTONIC, &arrival_time);
    usleep(1);
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    uint64_t PLEASE_WORK;
    PLEASE_WORK = ((end_time.tv_sec - arrival_time.tv_sec) * 1e9) + (end_time.tv_nsec - arrival_time.tv_nsec);
    printf("Nanoseconds elapsed: %ld\n", (PLEASE_WORK));

    return 0;
}