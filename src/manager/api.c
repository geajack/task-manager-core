#include "./api.h"

#include <dirent.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "stdlib.h"
#include "string.h"

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
    const int count = 5;
    StartedTask *tasks = malloc(count * sizeof(StartedTask));
    for (int i = 0; i < count; i++)
    {
        tasks[i].label = "Hello, world!";
    }
    tasks_list->tasks = tasks;
    tasks_list->count = count;

    return 0;
}

void started_task_to_file(StartedTask *task, char *file_path)
{
    int info_file = open(file_path, O_WRONLY | O_CREAT, FILE_PERMISSIONS);
    {
        char* field = ((task->launch_configuration).command);
        write(
            info_file,
            field,
            strlen(field)
        );
    }
    {
        char* field = ((task->launch_configuration).cwd);
        write(
            info_file,
            field,
            strlen(field)
        );
    }
    {
        char* field = task->label;
        write(
            info_file,
            field,
            strlen(field)
        );
    }
    close(info_file);
}