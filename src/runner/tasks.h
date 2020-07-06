#pragma once

#include <cJSON.h>

typedef struct TaskInfo
{
    cJSON *json;
} TaskInfo;

int get_task_info(char const* home, int task_id, TaskInfo *info);
int get_next_task_id(char const* home);
void start_new_task(char const* home, int task_id);
void add_task_file(char const* home, int task_id, int process_id);
int get_pid(TaskInfo *info);
TaskStatus get_status(TaskInfo *info);
int destroy_task_info(TaskInfo *info);
