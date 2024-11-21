#ifndef RM_H
#define RM_H

#include "global.h"
#define WCET_LEN 4

/**
 * @brief This function will calculate if the execution times are valid for Rate Monotonic Scheduling
 * @param sys_info SystemInfo instance
 * @param task_info TaskInfo instance
 */
void rm_scheduling(SystemInfo *sys_info, TaskInfo *task_info);

/**
 * @brief This function will calculate if the execution times are valid for Rate Monotonic Scheduling
 * @param sys_info SystemInfo instance
 * @param task_info TaskInfo instance
 * @param idx Index of the WCET array
 * @return 1 if valid, 0 if invalid
 */
int is_rm_valid(SystemInfo *sys_info, TaskInfo *task_info, int idx);

#endif