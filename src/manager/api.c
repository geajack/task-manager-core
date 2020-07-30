#include "./api.h"

#include <dirent.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "../runner/api.h"
#include "../repository.h"

static const int FILE_PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

void save_started_task(StartedTask *task, char *home);

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
    
    char started_tasks_directory[strlen(home) + strlen("/started")];
    strcpy(started_tasks_directory, home);
    strcat(started_tasks_directory, "/started");

    RepositoryEntry repository_entry;
    repository_create_entry(started_tasks_directory, &repository_entry);

    char* label = malloc(strlen(config->command) + 10);
    sprintf(label, "%s (%d)", config->command, task_id);

    StartedTask task;
    task.launch_configuration = *config;
    task.label = label;
    task.runner_id = task_id;

    save_started_task(&task, repository_entry.path);
    free(repository_entry.path);
}

int get_running_tasks(char const *home, StartedTasksList *tasks_list)
{
    StartedTask *tasks = malloc(100 * sizeof(StartedTask));
    char started_tasks_directory[strlen(home) + strlen("/started")];
    sprintf(started_tasks_directory, "%s%s", home, "/started");

    RepositoryEntryList entry_list;
    repository_get_all(started_tasks_directory, &entry_list);
    for (int i = 0; i < entry_list.count; i++)
    {
        char *path = entry_list.entries[i].path;
        load_started_task(&tasks[i], path);
    }

    tasks_list->tasks = tasks;
    tasks_list->count = entry_list.count;

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

load_started_task(StartedTask *task, const char* file_path)
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
    {
        fread(
            &task->runner_id,
            sizeof(task->runner_id),
            1,
            file
        );
    }
    close(file);
}

void save_started_task(StartedTask *task, char *file_path)
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
    {
        int field = task->runner_id;
        write(
            info_file,
            field,
            sizeof(field)
        );
    }
    close(info_file);
}