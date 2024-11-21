#ifndef GLOBAL_H
#define GLOBAL_H

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

float calculate_energy(int power, int time);

#endif