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

/************************************************************************************************
		These DEFINE statements represent the workload size of each task and
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
#define WORKLOAD4 100000

#define QUANTUM1 1250
#define QUANTUM2 1250
#define QUANTUM3 1250
#define QUANTUM4 1250

/************************************************************************************************
					DO NOT CHANGE THE FUNCTION IMPLEMENTATION
*************************************************************************************************/
void myfunction(int param)
{

	int i = 2;
	int j, k;

	while (i < param)
	{
		k = i;
		for (j = 2; j <= k; j++)
		{
			if (k % j == 0)
			{
				k = k / j;
				j--;
				if (k == 1)
				{
					break;
				}
			}
		}
		i++;
	}
}
/************************************************************************************************/

int main(int argc, char const *argv[])
{
	pid_t pid1, pid2, pid3, pid4;
	int running1, running2, running3, running4;

	pid1 = fork();

	if (pid1 == 0)
	{

		myfunction(WORKLOAD1);

		exit(0);
	}
	kill(pid1, SIGSTOP);

	pid2 = fork();

	if (pid2 == 0)
	{

		myfunction(WORKLOAD2);

		exit(0);
	}
	kill(pid2, SIGSTOP);

	pid3 = fork();

	if (pid3 == 0)
	{

		myfunction(WORKLOAD3);

		exit(0);
	}
	kill(pid3, SIGSTOP);

	pid4 = fork();

	if (pid4 == 0)
	{

		myfunction(WORKLOAD4);

		exit(0);
	}
	kill(pid4, SIGSTOP);

	/************************************************************************************************
		At this point, all  newly-created child processes are stopped, and ready for scheduling.
	*************************************************************************************************/

	/************************************************************************************************
		- Scheduling code starts here
	************************************************************************************************/
	/**
	 * Response time
	 */
	struct timespec arrival_time, end_time[4];
	clock_gettime(CLOCK_MONOTONIC, &arrival_time);
	/**
	 * Arrival Order:
	 * P1->P2->P3->P4
	 */
	kill(pid1, SIGCONT);
	waitpid(pid1, NULL, 0);
	clock_gettime(CLOCK_MONOTONIC, &end_time[0]);

	kill(pid2, SIGCONT);
	waitpid(pid2, NULL, 0);
	clock_gettime(CLOCK_MONOTONIC, &end_time[1]);

	// get end time

	kill(pid3, SIGCONT);
	waitpid(pid3, NULL, 0);
	clock_gettime(CLOCK_MONOTONIC, &end_time[2]);

	// get end time

	kill(pid4, SIGCONT);
	waitpid(pid4, NULL, 0);
	clock_gettime(CLOCK_MONOTONIC, &end_time[3]);
	float response_times[4];
	for (int i = 0; i < 4; i++)
	{
		response_times[i] = (((end_time[i].tv_sec - arrival_time.tv_sec) * 1e9) + (end_time[i].tv_nsec - arrival_time.tv_nsec)) * 1e-9;
		printf("Task %d Response Time: %.8fs\n", i + 1, response_times[i]);
	}

	// get end time
	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/

	return 0;
}