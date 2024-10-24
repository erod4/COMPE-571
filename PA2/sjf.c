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
#define WORKLOAD2 50000
#define WORKLOAD3 25000
#define WORKLOAD4 10000

#define QUANTUM1 1000
#define QUANTUM2 1000
#define QUANTUM3 1000
#define QUANTUM4 1000

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

	struct timeval creation_time[4]; // To store creation times
	struct timeval start_time[4];	 // To store start times
	double response_time[4];		 // To store response times

	gettimeofday(&creation_time[0], NULL);
	pid1 = fork();

	if (pid1 == 0)
	{

		myfunction(WORKLOAD1);

		exit(0);
	}
	kill(pid1, SIGSTOP);

	gettimeofday(&creation_time[1], NULL);
	pid2 = fork();

	if (pid2 == 0)
	{

		myfunction(WORKLOAD2);

		exit(0);
	}
	kill(pid2, SIGSTOP);

	gettimeofday(&creation_time[2], NULL);
	pid3 = fork();

	if (pid3 == 0)
	{

		myfunction(WORKLOAD3);

		exit(0);
	}
	kill(pid3, SIGSTOP);

	gettimeofday(&creation_time[3], NULL);
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
	 * Assuming execution time is workload size.
	 * SJF requires priority be set by execution time.
	 */
	struct Process
	{
		pid_t pid;
		long workload;
		int response_time_idx; // Keeps track of the response time index since we will sort below.
	};

	struct Process processes[4] =
		{
			{pid1, WORKLOAD1, 0},
			{pid2, WORKLOAD2, 1},
			{pid3, WORKLOAD3, 2},
			{pid4, WORKLOAD4, 3}};

	// Sort processes based on workload size (SJF scheduling)
	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			if (processes[i].workload > processes[j].workload)
			{
				struct Process temp = processes[i];
				processes[i] = processes[j];
				processes[j] = temp;
			}
		}
	}

	// index 0 should be highest priority -> run first.
	for (int i = 0; i < 4; i++)
	{
		// continue the process
		kill(processes[i].pid, SIGCONT);
		// wait for process to terminate since SJF is not preemptive.
		waitpid(processes[i].pid, NULL, 0);
		// Calculate response time in milliseconds (sec+ms converted to ms)
		gettimeofday(&start_time[processes[i].response_time_idx], NULL);

		response_time[processes[i].response_time_idx] = (start_time[processes[i].response_time_idx].tv_sec - creation_time[processes[i].response_time_idx].tv_sec) * 1000.0;
		response_time[processes[i].response_time_idx] += (start_time[processes[i].response_time_idx].tv_usec - creation_time[processes[i].response_time_idx].tv_usec) / 1000.0;
	}

	/************************************************************************************************
		- Scheduling code ends here
	************************************************************************************************/
	// Print response times for each process
	printf("Process Resonse Time:\n");
	printf("Process 1 (Workload %d): %.2f ms\n", WORKLOAD1, response_time[0]);
	printf("Process 2 (Workload %d): %.2f ms\n", WORKLOAD2, response_time[1]);
	printf("Process 3 (Workload %d): %.2f ms\n", WORKLOAD3, response_time[2]);
	printf("Process 4 (Workload %d): %.2f ms\n", WORKLOAD4, response_time[3]);
	return 0;
}