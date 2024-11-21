#include "edf.h"
#include "global.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

const int f_cpus_MHz[] = {1188, 918, 648, 384};
void edf_scheduling(SystemInfo *sys_info, TaskInfo *task_info)
{
    int is_edf_valid_for_idx[WCET_LEN];
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
    int valid_task_count = 0;

    for (int i = 0; i < WCET_LEN; i++)
    {
        // store which index are valid
        is_edf_valid_for_idx[i] = is_edf_valid(sys_info, task_info, i);
        if (is_edf_valid_for_idx[i])
        {
            valid_task_count++;
        }
    }
    /*
    At this point we can start calculating scheduling.
    */

    TaskInfo valid_tasks[sys_info->num_tasks];
    // Store all tasks in the valid_tasks array
    for (int i = 0; i < sys_info->num_tasks; i++)
    {
        valid_tasks[i] = task_info[i];
    }

    // Sort tasks by deadline
    qsort(valid_tasks, sys_info->num_tasks, sizeof(TaskInfo), compare_by_deadline_desc);

    // Loop through each valid WCET index
    for (int wcet_index = 0; wcet_index < WCET_LEN; wcet_index++)
    {
        // Skip WCET columns that are not valid
        if (is_edf_valid_for_idx[wcet_index] == 0)
        {
            continue;
        }

        printf("\nEDF Execution Schedule for WCET Index %d (Valid):\n", wcet_index + 1);
        printf("Start Time(s)\tTask\tRun Time(s)\tFrequency\tEnergy(J)\n");

        int current_time = 0;
        int remaining_time[sys_info->num_tasks];
        int absolute_deadline[sys_info->num_tasks];

        // Initialize remaining time and absolute deadlines for all tasks
        for (int i = 0; i < sys_info->num_tasks; i++)
        {
            remaining_time[i] = valid_tasks[i].wcet[wcet_index];
            absolute_deadline[i] = valid_tasks[i].task_deadline;
        }

        int last_task = -1;      // Tracks the last executed task (-1 for idle)
        int task_start_time = 0; // Start time of the currently executing task or idle period

        while (current_time < sys_info->execution_time)
        {
            int selected_task = -1;
            int earliest_deadline = 1000000;

            // Find the task with the earliest absolute deadline that still has execution time remaining
            for (int i = 0; i < sys_info->num_tasks; i++)
            {
                if (remaining_time[i] > 0 && absolute_deadline[i] < earliest_deadline)
                {
                    selected_task = i;
                    earliest_deadline = absolute_deadline[i];
                }
            }

            if (selected_task != -1)
            {
                // If switching from idle or another task, print previous idle/task run time
                if (last_task != selected_task)
                {
                    int run_time = current_time - task_start_time;
                    if (last_task == -1)
                    {
                        // Print idle period
                        int power = sys_info->idle_power;
                        float energy = calculate_energy(power, run_time);
                        printf("%d\t\tIDLE\t%d\t\tIDLE\t\t%.4f\n", task_start_time, run_time, energy);
                    }
                    else
                    {
                        // Print task execution
                        int power = sys_info->active_power[wcet_index];
                        float energy = calculate_energy(power, run_time);
                        printf("%d\t\t%s\t%d\t\t%dMHz\t\t%.4f\n", task_start_time, valid_tasks[last_task].task_name, run_time, f_cpus_MHz[wcet_index], energy);
                    }
                    task_start_time = current_time;
                    last_task = selected_task;
                }

                // Execute the selected task for one time unit
                remaining_time[selected_task]--;
            }
            else
            {
                // If switching from a task to idle, print the previous task's run time
                if (last_task != -1)
                {
                    int run_time = current_time - task_start_time;
                    int power = sys_info->active_power[wcet_index];
                    float energy = calculate_energy(power, run_time);
                    printf("%d\t\t%s\t%d\t\t%dMHz\t\t%.4f\n", task_start_time, valid_tasks[last_task].task_name, run_time, f_cpus_MHz[wcet_index], energy);
                    task_start_time = current_time;
                    last_task = -1; // Now idle
                }
            }

            // Increment time
            current_time++;

            // Check for period completion and reset tasks
            for (int i = 0; i < sys_info->num_tasks; i++)
            {
                if (current_time % valid_tasks[i].task_deadline == 0)
                {
                    remaining_time[i] = valid_tasks[i].wcet[wcet_index];                // Reset WCET for this period
                    absolute_deadline[i] = current_time + valid_tasks[i].task_deadline; // Update the absolute deadline
                }
            }
        }

        // Print the last task's or idle time's runtime if the loop ends
        int run_time = current_time - task_start_time;
        if (last_task == -1)
        {
            printf("%d\t\tIDLE\t%d\t\tIDLE\t\t-\n", task_start_time, run_time);
        }
        else
        {
            int power = sys_info->active_power[wcet_index];
            float energy = calculate_energy(power, run_time);
            printf("%d\t\t%s\t%d\t\t%dMHz\t\t%.4f\n", task_start_time, valid_tasks[last_task].task_name, run_time, f_cpus_MHz[wcet_index], energy);
        }
    }
}

int is_edf_valid(SystemInfo *sys_info, TaskInfo *task_info, int idx)
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

    printf("Total Utilization: %.4f\n", utilization_sum);

    // if valid index value will be 1 otherwise 0 for invalid
    if (utilization_sum < 1.00)
    {
        printf("Execution Times of Column %d Are Valid\n\n", idx + 1);
        return 1;
    }
    printf("Execution Times of Column %d Are Not Valid\n\n", idx + 1);
    return 0;
}

int compare_by_deadline_desc(const void *a, const void *b)
{
    TaskInfo *taskA = (TaskInfo *)a;
    TaskInfo *taskB = (TaskInfo *)b;
    return taskA->task_deadline - taskB->task_deadline;
}