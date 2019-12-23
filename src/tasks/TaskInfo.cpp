#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/file.h>

#include "TaskInfo.h"

const int PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

TaskInfo::TaskInfo(std::string *filepath)
{
    this->filepath = filepath;
}

TaskInfo::~TaskInfo()
{
    delete this->filepath;
}

TaskStatus* TaskInfo::get_status()
{
    return NULL;
}

int TaskInfo::get_pid()
{
    char buffer[10];
    {
        int file = open(this->filepath->c_str(), O_RDONLY);
        int n_bytes = read(file, buffer, 10);
        close(file);
        buffer[n_bytes] = 0;
    }
    int process_id;
    {
        char *_;
        process_id = strtol(buffer, &_, 10);
    }

    return (int) process_id;
}


void create_task_file(char const* home, int task_id, int process_id)
{
    const char* task_info_template = "%d";
    char task_info[10];
    sprintf(task_info, task_info_template, process_id);

    int info_file;
    {
        char info_file_name[10];
        sprintf(info_file_name, "/tasks/%d", task_id);
        std::string info_file_path = std::string(home);
        info_file_path.append(info_file_name);
        info_file = open(info_file_path.c_str(), O_RDWR | O_CREAT, PERMISSIONS);
    }
    write(info_file, task_info, strlen(task_info));
    close(info_file);
}