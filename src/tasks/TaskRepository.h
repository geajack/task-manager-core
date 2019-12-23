#pragma once

#include "TaskInfo.h"

class TaskRepository
{
private:
    char const* home;
public:
    TaskRepository(char const* home);
    ~TaskRepository();
    TaskInfo* get_task_info(int task_id);
    int get_next_task_id();
    void create_new_task(int task_id);
};
