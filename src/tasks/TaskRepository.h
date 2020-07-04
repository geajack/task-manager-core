#pragma once

#include "TaskInfo.h"

class TaskRepository
{
private:
    char const* home;
public:
    TaskRepository(char const* home);
    ~TaskRepository();
    int get_task_info(int task_id, TaskInfo *info);
    int get_next_task_id();
    void start_new_task(int task_id);
    void add_task_file(int, int);
};
