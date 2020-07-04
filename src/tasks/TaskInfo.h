#pragma once

#include <cJSON.h>

#include "../tasks.h"

typedef struct TaskInfo
{
    cJSON *json;
} TaskInfo;

int task_info_from_file(char const *filepath, TaskInfo *info);

int destroy_task_info(TaskInfo *info);

TaskStatus get_status(TaskInfo *info);

int get_pid(TaskInfo *info);

void create_task_file(char const* home, int task_id, int process_id);