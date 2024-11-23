#include "rm.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ee_rm_scheduling(SystemInfo *sys_info, TaskInfo *task_info)
{
}

void rm_scheduling(SystemInfo *sys_info, TaskInfo *task_info)
{
    int is_rm_valid_for_idx[WCET_LEN];
    // loop through all tasks WCET and calculate if the utilization is less than rms bound
    //  Σ Ci/Ti = n*(2^(1/n)-1)
    /**
    *    i  0    1      2       3
    w1 520  53   66     89     141
    w2 220  40   50     67     114
    w3 500  104  134    184    313
    w4 200  57   74     103    175
    w5 300  35   45     62     104
    */
    for (int i = 0; i < WCET_LEN; i++)
    {
        // store which index are valid
        is_rm_valid_for_idx[i] = is_rm_valid(sys_info, task_info, i);
    }
    /*
    At this point we can start calculating scheduling.
    */

    // loop through each column and calculate priorities based on period
    for (int i = 0; i < WCET_LEN; i++)
    {
        // if rm schedule is valid compute schedule
        if (is_rm_valid_for_idx[i])
        {
        }
    }
}

int is_rm_valid(SystemInfo *sys_info, TaskInfo *task_info, int idx)
{
    /*
      idx  0
   w1 520  53
   w2 220  40
   w3 500  104
   w4 200  57
   w5 300  35
   */

    // store utlization total
    float utilization_sum = 0;
    // loop through each task and add onto util sum (Ci/Ti)
    for (int i = 0; i < sys_info->num_tasks; i++)
    {
        utilization_sum += (float)task_info[i].wcet[idx] / task_info[i].task_deadline;
    }
    // calculate rms bound
    float rms_bound = sys_info->num_tasks * (pow(2.0, 1.0 / sys_info->num_tasks) - 1);

    printf("Total Utilization: %.2f\n", utilization_sum);
    printf("RMS Utilization Bound: %.2f\n", rms_bound);

    // if valid index value will be 1 otherwise 0 for invalid
    if (utilization_sum < rms_bound)
    {
        printf("Execution Times of Column %d Are Valid\n\n", idx + 1);
        return 1;
    }
    printf("Execution Times of Column %d Are Not Valid\n\n", idx + 1);
    return 0;
}