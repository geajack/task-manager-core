#pragma once

#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "api.h"
#include "TaskInfo.c"

void add_task_file(char const* home, int task_id, int process_id);
void start_new_task(char const* home, int task_id);
int get_task_info(char const* home, int task_id, TaskInfo *info);
int get_next_task_id(char const* home);
void create_task_file(char const* home, TaskInfo *info, int task_id);

void add_task_file(char const* home, int task_id, int process_id)
{
    TaskInfo info;
    create_task_info_for_process(&info, process_id);
    create_task_file(home, &info, task_id);
    destroy_task_info(&info);
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
    task_info_from_file(info, path);
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

TaskStatus get_status(TaskInfo *info)
{
    int process_id = task_info_get_process_id(info);
    int start_time = task_info_get_start_time(info);
    
    struct stat file_details;
    int io_error;
    {
        char process_file_path[20];
        sprintf(process_file_path, "/proc/%d", process_id);
        io_error = lstat(process_file_path, &file_details);
    }

    if (!io_error)
    {
        if (start_time == file_details.st_ctim.tv_nsec)
        {
            return RUNNING;
        }
    }

    return STOPPED;
}

int get_pid(TaskInfo *info)
{
    int process_id = task_info_get_process_id(info);
    return process_id;
}

void create_task_file(char const* home, TaskInfo *info, int task_id)
{
    const int prefix_length = strlen(home) + 7;
    const int n_digits = 10;
    char info_file_path[prefix_length + n_digits];
    sprintf(info_file_path, "%s/tasks/%d", home, task_id);

    task_info_to_file(info, info_file_path);
}