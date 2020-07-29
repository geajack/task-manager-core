#include "./api.h"

#include <dirent.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "../runner/api.h"

static const int FILE_PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

void started_task_to_file(StartedTask *task, char *home);

int start(char const *home, LaunchConfiguration *config)
{
    char runner_home[strlen(home) + strlen("/runner")];
    strcpy(runner_home, home);
    strcat(runner_home, "/runner");
    int task_id = run(
        runner_home,
        config->command,
        config->cwd
    );
    
    int next_task_id = 0;
    char started_tasks_directory[strlen(home) + strlen("/started")];
    strcpy(started_tasks_directory, home);
    strcat(started_tasks_directory, "/started");
    {
        DIR* tasks_directory = opendir(started_tasks_directory);
        if (tasks_directory)
        {
            struct dirent *file;
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
    }

    char file_path[strlen(started_tasks_directory) + 10];
    sprintf(file_path, "%s/%d", started_tasks_directory, next_task_id);

    StartedTask task;
    task.launch_configuration = *config;
    task.label = config->command;

    started_task_to_file(&task, file_path);
}

int get_running_tasks(char const *home, StartedTasksList *tasks_list)
{
    StartedTask *tasks = malloc(100 * sizeof(StartedTask));
    char started_tasks_directory[strlen(home) + strlen("/started")];
    sprintf(started_tasks_directory, "%s%s", home, "/started");
    int n_tasks;
    char file_path[strlen(started_tasks_directory) + 256];
    {
        DIR* tasks_directory = opendir(started_tasks_directory);
        if (tasks_directory)
        {
            struct dirent *file;
            int i = 0;
            while ((file = readdir(tasks_directory)) != NULL)
            {
                if (file->d_type == 8)
                {
                    sprintf(file_path, "%s/%s", started_tasks_directory, file->d_name);
                    started_task_from_file(&tasks[i], file_path);
                    i += 1;
                }
            }
            n_tasks = i;
            closedir(tasks_directory);
        }
    }

    tasks_list->tasks = tasks;
    tasks_list->count = n_tasks;

    return 0;
}

int read_string_from_file(char* buffer, int max_bytes, FILE *file)
{
    char character;
    int i = 0;
    int n_bytes_read = 0;
    while (n_bytes_read < max_bytes)
    {
        character = fgetc(file);
        buffer[i] = character;
        i += 1;
        if (character == 0)
        {
            return 0;
        }
    }
    return 1;    
}

started_task_from_file(StartedTask *task, const char* file_path)
{    
    FILE *file = fopen(file_path, "rb");
    {
        char* buffer = malloc(256);
        task->launch_configuration.command = buffer;
        read_string_from_file(
            buffer,
            256,
            file
        );
    }
    {
        char* buffer = malloc(256);
        task->launch_configuration.cwd = buffer;
        read_string_from_file(
            buffer,
            256,
            file
        );
    }
    {
        char* buffer = malloc(256);
        task->label = buffer;
        read_string_from_file(
            buffer,
            256,
            file
        );
    }
    close(file);
}

void started_task_to_file(StartedTask *task, char *file_path)
{
    int info_file = open(file_path, O_WRONLY | O_CREAT, FILE_PERMISSIONS);
    {
        char* field = ((task->launch_configuration).command);
        write(
            info_file,
            field,
            strlen(field) + 1
        );
    }
    {
        char* field = ((task->launch_configuration).cwd);
        write(
            info_file,
            field,
            strlen(field) + 1
        );
    }
    {
        char* field = task->label;
        write(
            info_file,
            field,
            strlen(field) + 1
        );
    }
    close(info_file);
}