#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pids[3];
    int i;

    for (i = 0; i < 3; i++)
    {
        pids[i] = fork();
    }
    printf("Hello\n");

    // You can then determine uniquely identify the all available processes as a combination of pids array
}
