#ifndef EDF_H
#define EDF_H

#include "global.h"
#define WCET_LEN 4
#define ALPHA 0.75

/**
 * @brief This function will calculate if the execution times are valid for Earliest Deadline First Scheduling for Energy Efficiency
 * @param sys_info SystemInfo instance
 * @param task_info TaskInfo instance
 */
void ee_edf_scheduling(SystemInfo *sys_info, TaskInfo *task_info);

/**
 * @brief This function will calculate if the execution times are valid for Earliest Deadline First Scheduling
 * @param sys_info SystemInfo instance
 * @param task_info TaskInfo instance
 */
void edf_scheduling(SystemInfo *sys_info, TaskInfo *task_info);

/**
 * @brief This function will calculate if the execution times are valid for Earliest Deadline First Scheduling
 * @param sys_info SystemInfo instance
 * @param task_info TaskInfo instance
 * @param idx Index of the WCET array
 * @return 1 if valid, 0 if invalid
 */
int is_edf_valid(SystemInfo *sys_info, TaskInfo *task_info, int idx, float alpha);

/**
 * @brief This function will compare two TaskInfo instances by deadline
 * @param a TaskInfo instance
 * @param b TaskInfo instance
 * @return 1 if a is greater, -1 if b is greater, 0 if equal
 */
int compare_by_deadline_desc(const void *a, const void *b);

#endif