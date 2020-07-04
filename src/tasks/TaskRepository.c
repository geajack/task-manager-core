#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "../tasks.h"
#include "TaskInfo.h"
#include "TaskRepository.h"

static const int PERMISSIONS = 
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
        int path_length = strlen(home) + 1 + 10 + 4;
        char path[path_length];
        sprintf(path, "%s/%d.log", home, task_id);
        output_file = open(path, O_RDWR | O_CREAT, PERMISSIONS);
    }
    dup2(output_file, 1);
    dup2(output_file, 2);
    close(output_file);
}

int get_task_info(char const* home, int task_id, TaskInfo *info)
{
    int path_length = strlen(home) + 7 + 10;
    char path[path_length];
    sprintf(path, "%s/tasks/%d", home, task_id);
    task_info_from_file(path, info);
    return 0;
}

int get_next_task_id(char const* home)
{
    struct dirent *file;

    int path_length = strlen(home) + 7;
    char path[path_length];
    sprintf(path, "%s/tasks", home);

    int next_task_id = 0;
    DIR* tasks_directory = opendir(path);
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