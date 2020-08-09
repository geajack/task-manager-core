#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "api.h"
#include "../repository.h"
#include "TaskInfo.i"

void add_task_file(char const* home, int task_id, int process_id);
void capture_logs(char const* home, int task_id);
int get_task_info(char const* home, int task_id, TaskInfo *info);
int get_next_task_id(char const* home);

void add_task_file(char const* home, int task_id, int process_id)
{
    TaskInfo info;
    create_task_info_for_process(&info, process_id);
    
    const int prefix_length = strlen(home) + 7;
    const int n_digits = 10;
    char info_file_path[prefix_length + n_digits];
    sprintf(info_file_path, "%s/tasks/%d", home, task_id);
    task_info_to_file(&info, info_file_path);
    
    destroy_task_info(&info);
}

void capture_logs(char const* home, int task_id)
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
    char *tasks_home = malloc(strlen(home) + 6);
    sprintf(tasks_home, "%s/tasks", home);
    char *path = repository_get_file_path_for_id(tasks_home, task_id);
    task_info_from_file(info, path);
    free(path);
    free(tasks_home);
    return 0;
}

int get_next_task_id(char const* home)
{
    int path_length = strlen(home) + 7;
    char path[path_length];
    sprintf(path, "%s/tasks", home);

    return repository_get_next_id(path);
}