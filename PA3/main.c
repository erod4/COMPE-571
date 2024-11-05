#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

    fclose(file); // Close the file
    return 0;
}
