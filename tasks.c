#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/dir.h>

#include "tasks.h"

int get_next_task_id(char* home);

const int PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

int run(char* home, char* command, char* arguments[], int n_arguments, char* cwd)
{
    int task_id = get_next_task_id(home);

    chdir(home);

    int fork_result = fork();

    if (fork_result > 1)
    {
        int process_id = fork_result;
        static char* task_info_template = "%d";
        char task_info[10];
        sprintf(task_info, task_info_template, process_id);

        char info_file_name[10];
        sprintf(info_file_name, "tasks/%d", task_id);
        int info_file = open(info_file_name, O_RDWR | O_CREAT, PERMISSIONS);
        write(info_file, task_info, strlen(task_info));
        close(info_file);

        return task_id;
    }
    else if (fork_result == 0)
    {
        int output_file;
        int error_file;
        {            
            char output_file_name[10+7];
            char error_file_name[10+7];
            sprintf(output_file_name, "%d.stdout", task_id);
            sprintf(error_file_name, "%d.stderr", task_id);
            output_file = open(output_file_name, O_RDWR | O_CREAT, PERMISSIONS);
            error_file = open(error_file_name, O_RDWR | O_CREAT, PERMISSIONS);
        }
        dup2(output_file, 1);
        dup2(error_file, 2);
        close(output_file);
        close(error_file);

        char* exec_arguments[n_arguments + 2];
        {
            exec_arguments[0] = command;
            memcpy(
                &exec_arguments[1],
                arguments,
                sizeof(char*) * n_arguments
            );
            exec_arguments[n_arguments + 1] = NULL;
        }

        execvp(command, exec_arguments);
    }
    else
    {
        return -1;
    }
}

void stop(int task_id)
{

}

TaskStatus status(int task_id)
{
}

char* logs(int task_id)
{

}

int get_next_task_id(char* home_path)
{
    struct dirent *file;

    char path[strlen(home_path) + 7];
    strcpy(path, home_path);
    strcat(path, "/tasks");

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