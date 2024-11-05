#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WCET_LEN 4

typedef struct
{
    int num_tasks;
    int execution_time;
    int active_power[4];
    int idle_power;
} SystemInfo;

typedef struct
{
    char task_name[10];
    int task_deadline;
    int wcet[4];

} TaskInfo;

void rm_scheduling(SystemInfo *sys_info, TaskInfo *task_info);
void edf_scheduling(SystemInfo *sys_info, TaskInfo *task_info);

int is_rm_valid(SystemInfo *sys_info, TaskInfo *task_info, int idx);
int is_edf_valid(SystemInfo *sys_info, TaskInfo *task_info, int idx);

int main(int argc, char const *argv[])
{
    FILE *file;

    // Check to see if enough args were passed
    if (argc < 3)
    {
        printf("Please provide the name of the file followed by the scheduling algorithm.\n");
        printf("Example: ./main test.txt EDF\n");
        return 1;
    }

    // check if file exists and read file
    file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Could not find file, ensure it is at base directory and a .txt file.\n\r");
        return 1;
    }

    // returns 0 if match otherwise 1
    int is_EDF_selected = strcmp(argv[2], "EDF");
    int is_RM_selected = strcmp(argv[2], "RM");

    if (is_EDF_selected && is_RM_selected)
    {
        printf("Please Select either EDF or RM.\n\r");
        return 1;
    }
    //////////////////////////////////////
    //  Error Checking Ends Here!       //
    /////////////////////////////////////

    /**
     * This section parses document and stores system into a SystemInfo instance and stores each task info into its respective TaskInfo instance.
     */

    // SystemInfo instance to store the first line
    SystemInfo system_info;

    fscanf(file, "%d %d %d %d %d %d %d",
           &system_info.num_tasks,
           &system_info.execution_time,
           &system_info.active_power[0],
           &system_info.active_power[1],
           &system_info.active_power[2],
           &system_info.active_power[3],
           &system_info.idle_power);

    // Output parsed data to verify correctness
    printf("System Information:\n");
    printf("Number of tasks: %d\n", system_info.num_tasks);
    printf("Execution time: %d seconds\n", system_info.execution_time);
    printf("Active CPU Power at 1188 MHz: %d\n", system_info.active_power[0]);
    printf("Active CPU Power at 918 MHz: %d\n", system_info.active_power[1]);
    printf("Active CPU Power at 648 MHz: %d\n", system_info.active_power[2]);
    printf("Active CPU Power at 384 MHz: %d\n", system_info.active_power[3]);
    printf("Idle CPU Power: %d\n\n", system_info.idle_power);

    // Array of TaskInfo instances to store each tasks information
    TaskInfo task_info[system_info.num_tasks];

    // loop through each task in file and store data
    for (int i = 0; i < system_info.num_tasks; i++)
    {
        fscanf(file, "%s %d %d %d %d %d",
               task_info[i].task_name,
               &task_info[i].task_deadline,
               &task_info[i].wcet[0],
               &task_info[i].wcet[1],
               &task_info[i].wcet[2],
               &task_info[i].wcet[3]);
    }

    for (int i = 0; i < system_info.num_tasks; i++)
    {
        printf("\n\nTask Name: %s\n", task_info[i].task_name);
        printf("Task Deadline: %d\n", task_info[i].task_deadline);
        printf("Task WCET 1: %d\n", task_info[i].wcet[0]);
        printf("Task WCET 2: %d\n", task_info[i].wcet[1]);
        printf("Task WCET 3: %d\n", task_info[i].wcet[2]);
        printf("Task WCET 4: %d\n\n", task_info[i].wcet[3]);
    }
    // if rm selected execute rm
    if (is_RM_selected == 0)
    {
        rm_scheduling(&system_info, task_info);
    }
    else if (is_EDF_selected == 0)
    {
    }

    fclose(file); // Close the file
    return 0;
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
    for (int i = 0; i < WCET_LEN; i++)
    {
        // store which index are valid
        is_edf_valid_for_idx[i] = is_edf_valid(sys_info, task_info, i);
    }
    /*
    At this point we can start calculating scheduling.
    */

    // loop through each column and calculate priorities based on period
    for (int i = 0; i < WCET_LEN; i++)
    {
        // if rm schedule is valid compute schedule
        if (is_edf_valid_for_idx[i])
        {
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

    printf("Total Utilization: %.2f\n", utilization_sum);

    // if valid index value will be 1 otherwise 0 for invalid
    if (utilization_sum < 1.00)
    {
        printf("Execution Times of Column %d Are Valid\n\n", idx + 1);
        return 1;
    }
    printf("Execution Times of Column %d Are Not Valid\n\n", idx + 1);
    return 0;
}