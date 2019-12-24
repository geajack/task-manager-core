#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/signal.h>

#include "tasks.h"
#include "tasks/TaskInfo.h"
#include "tasks/TaskRepository.h"

int run(char const* home, char* command, char* arguments[], int n_arguments, char const* cwd)
{
    TaskRepository repository = TaskRepository(home);
    int task_id = repository.get_next_task_id();

    int fork_result = fork();

    if (fork_result > 1)
    {
        int process_id = fork_result;
        create_task_file(home, task_id, process_id);
        return task_id;
    }
    else if (fork_result == 0)
    {
        repository.create_new_task(task_id);

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

    return -1;
}

void stop(char const* home, int task_id)
{
    TaskRepository *repository = new TaskRepository(home);
    TaskInfo *info = repository->get_task_info(task_id);
    int kill_result = kill(info->get_pid(), SIGKILL);
}

TaskStatus status(char const* home, int task_id)
{
    TaskRepository *repository = new TaskRepository(home);
    TaskInfo *info = repository->get_task_info(task_id);
    return info->get_status();
}

char* logs(char const* home, int task_id)
{
    char *path = (char*) malloc(strlen(home) + 10);
    sprintf(path, "%s/%d.log", home, task_id);
    return path;
}