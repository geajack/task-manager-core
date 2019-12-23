#pragma once

#include <string>

#include "../tasks.h"

class TaskInfo
{
private:
    std::string *filepath;
public:
    TaskInfo(std::string *filepath);
    ~TaskInfo();
    TaskStatus* get_status();
    int get_pid();
};

void create_task_file(char const* home, int task_id, int process_id);