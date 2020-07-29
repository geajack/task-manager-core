#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/signal.h>

#include "api.h"
#include "tasks.c"

int run(char const* home, char* command, char const* cwd)
{
    int task_id = get_next_task_id(home);

    int fork_result = fork();

    if (fork_result > 1)
    {
        int process_id = fork_result;
        add_task_file(home, task_id, process_id);
        return task_id;
    }
    else if (fork_result == 0)
    {
        start_new_task(home, task_id);

        int command_length = strlen(command);
        char wrapped_command[5 + command_length + 1];
        strcpy(wrapped_command, "exec ");
        strcat(wrapped_command, command);

        char *exec_arguments[] = {
            "/bin/sh",
            "-c",
            wrapped_command,
            0
        };

        execvp("/bin/sh", exec_arguments);
    }

    return -1;
}

void stop(char const* home, int task_id)
{
    TaskInfo info;
    get_task_info(home, task_id, &info);
    int kill_result = kill(get_pid(&info), SIGKILL);
    destroy_task_info(&info);
}

TaskStatus status(char const* home, int task_id)
{
    TaskInfo info;
    get_task_info(home, task_id, &info);
    TaskStatus status = get_status(&info);
    destroy_task_info(&info);
    return status;
}

void logs(char const *home, int task_id, char *buffer)
{
    sprintf(buffer, "%s/%d.log", home, task_id);
}