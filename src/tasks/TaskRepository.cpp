#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "../tasks.h"
#include "TaskInfo.h"
#include "TaskRepository.h"

const int PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

void add_task_file(char const* home, int task_id, int process_id)
{
    create_task_file(home, task_id, process_id);
}

void start_new_task(char const* home, int task_id)
{
    int output_file;
    {
        char output_file_name[10+7];
        sprintf(output_file_name, "/%d.log", task_id);
        std::string output_file_path = std::string(home);
        output_file_path.append(output_file_name);
        output_file = open(output_file_path.c_str(), O_RDWR | O_CREAT, PERMISSIONS);
    }
    dup2(output_file, 1);
    dup2(output_file, 2);
    close(output_file);
}

int get_task_info(char const* home, int task_id, TaskInfo *info)
{
    std::string *path = new std::string(home);
    {
        char filename[6 + 10];
        sprintf(filename, "/tasks/%d", task_id);
        path->append(filename);
    }
    task_info_from_file(path->c_str(), info);
    delete path;
    return 0;
}

int get_next_task_id(char const* home)
{
    struct dirent *file;

    std::string path = std::string(home);
    {
        path.append("/tasks");
    }

    int next_task_id = 0;
    DIR* tasks_directory = opendir(path.c_str());
    if (tasks_directory)
    {
        while ((file = readdir(tasks_directory)) != NULL)
        {
            if (file->d_type == 8)
            {
                char* _;
                int task_id = strtol(file->d_name, &_, 10);

                if (task_id >= next_task_id)
                {
                    next_task_id = task_id + 1;
                }
            }
        }
        closedir(tasks_directory);
    }

    return next_task_id;
}