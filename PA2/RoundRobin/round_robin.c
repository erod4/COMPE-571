#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

/************************************************************************************************
		These DEFINE statements represent the workload size of each task and
		the time quantum values for Round Robin scheduling for each task.
*************************************************************************************************/

#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
#define WORKLOAD4 100000

#define QUANTUM1 2000
#define QUANTUM2 2000
#define QUANTUM3 2000
#define QUANTUM4 2000

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
		- Below is a sample schedule. (which scheduling algorithm is this?)
		- For the assignment purposes, you have to replace this part with the other scheduling methods
		to be implemented.
	************************************************************************************************/
	running1 = 1;
	running2 = 1;
	running3 = 1;
	running4 = 1;

	struct timespec arrival_time, end_time[4];
	struct timespec csEnd, csStart;
	float totalCSTime = 0.0;
	int firstRun = 1;
	clock_gettime(CLOCK_MONOTONIC, &arrival_time);

	while (running1 > 0 || running2 > 0 || running3 > 0 || running4 > 0)
	{
		if (running1 > 0)
		{

			kill(pid1, SIGCONT);
			clock_gettime(CLOCK_MONOTONIC, &csEnd);
			usleep(QUANTUM1);
			kill(pid1, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &end_time[0]);

			if (firstRun != 1)
			{
				totalCSTime += (((csEnd.tv_sec - csStart.tv_sec) * 1e9) + (csEnd.tv_nsec - csStart.tv_nsec)) * 1e-9;
			}

			firstRun = 0;
			clock_gettime(CLOCK_MONOTONIC, &csStart);
		}

		if (running2 > 0)
		{

			kill(pid2, SIGCONT);
			clock_gettime(CLOCK_MONOTONIC, &csEnd);
			usleep(QUANTUM2);
			kill(pid2, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &end_time[1]);
			totalCSTime += (((csEnd.tv_sec - csStart.tv_sec) * 1e9) + (csEnd.tv_nsec - csStart.tv_nsec)) * 1e-9;
			clock_gettime(CLOCK_MONOTONIC, &csStart);
		}

		if (running3 > 0)
		{

			kill(pid3, SIGCONT);
			clock_gettime(CLOCK_MONOTONIC, &csEnd);
			usleep(QUANTUM3);
			kill(pid3, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &end_time[2]);
			totalCSTime += (((csEnd.tv_sec - csStart.tv_sec) * 1e9) + (csEnd.tv_nsec - csStart.tv_nsec)) * 1e-9;

			clock_gettime(CLOCK_MONOTONIC, &csStart);
		}

		if (running4 > 0)
		{

			kill(pid4, SIGCONT);
			clock_gettime(CLOCK_MONOTONIC, &csEnd);
			usleep(QUANTUM4);
			kill(pid4, SIGSTOP);
			clock_gettime(CLOCK_MONOTONIC, &end_time[3]);
			totalCSTime += (((csEnd.tv_sec - csStart.tv_sec) * 1e9) + (csEnd.tv_nsec - csStart.tv_nsec)) * 1e-9;
			clock_gettime(CLOCK_MONOTONIC, &csStart);
		}

		waitpid(pid1, &running1, WNOHANG);
		waitpid(pid2, &running2, WNOHANG);
		waitpid(pid3, &running3, WNOHANG);
		waitpid(pid4, &running4, WNOHANG);
	}

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/
	float response_times[4];
	printf("Quantum Size %d\n", QUANTUM1);
	for (int i = 0; i < 4; i++)
	{
		response_times[i] = (((end_time[i].tv_sec - arrival_time.tv_sec) * 1e9) + (end_time[i].tv_nsec - arrival_time.tv_nsec)) * 1e-9;
		printf("Task %d Response Time: %.8fs\n", i + 1, response_times[i]);
	}

	printf("Total Context Switch Time: %.8fs\n", totalCSTime);

	return 0;
}