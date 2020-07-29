#include "./api.h"

#include "stdlib.h"

#include "../runner/api.h"

int start(char const *home, TaskConfig *config)
{
    return run(
        home,
        config->command,
        config->arguments,
        config->n_arguments,
        config->cwd
    );
}

get_running_tasks(char const *home, TasksList *tasks_list)
{
    const int count = 5;
    TaskHeader *headers = malloc(count * sizeof(TaskHeader));
    for (int i = 0; i < count; i++)
    {
        headers[i].label = "Hello, world!";
    }
    tasks_list->tasks = headers;
    tasks_list->count = count;
}