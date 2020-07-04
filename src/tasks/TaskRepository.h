#pragma once

#include "TaskInfo.h"

int get_task_info(char const* home, int task_id, TaskInfo *info);
int get_next_task_id(char const* home);
void start_new_task(char const* home, int task_id);
void add_task_file(char const* home, int task_id, int process_id);
