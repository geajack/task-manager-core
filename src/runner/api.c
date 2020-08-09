#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/unistd.h>
#include <sys/signal.h>

#include "api.h"
#include "tasks.i"

int run(char const* home, char* command, char const* cwd)
{
    int task_id = get_next_task_id(home);

    // Block SIGINT
    sigset_t signal_mask;
    sigset_t old_mask;
    sigemptyset(&signal_mask);
    sigaddset(&signal_mask, SIGINT);
    sigprocmask(SIG_BLOCK, &signal_mask, &old_mask);

    pid_t fork_result = fork();

    if (fork_result > 1)
    {        
        pid_t process_id = fork_result;

        // Set child PGID to its PID - avoids race condition with child
        setpgid(process_id, process_id);

        // Unblock SIGINT
        sigprocmask(SIG_SETMASK, &old_mask, 0);
        
        add_task_file(home, task_id, process_id);
        return task_id;
    }
    else if (fork_result == 0)
    {
        // Set my PGID to my PID - avoids race condition with parent
        setpgid(0, 0);

        capture_logs(home, task_id);

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
        abort();
    }

    return -1;
}

void stop(char const* home, int task_id)
{
    TaskInfo info;
    get_task_info(home, task_id, &info);
    int kill_result = kill(task_info_get_process_id(&info), SIGKILL);
    destroy_task_info(&info);
}

TaskStatus status(char const* home, int task_id)
{
    TaskInfo info;
    get_task_info(home, task_id, &info);
    TaskStatus status = task_info_get_status(&info);
    destroy_task_info(&info);
    return status;
}

void logs(char const *home, int task_id, char *buffer)
{
    sprintf(buffer, "%s/%d.log", home, task_id);
}