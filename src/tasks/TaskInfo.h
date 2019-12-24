#pragma once

#include <string>

#include <rapidjson/document.h>

#include "../tasks.h"

class TaskInfo
{
private:
    rapidjson::Document *document;
public:
    TaskInfo(std::string *filepath);
    ~TaskInfo();
    TaskStatus get_status();
    int get_pid();
};

void create_task_file(char const* home, int task_id, int process_id);