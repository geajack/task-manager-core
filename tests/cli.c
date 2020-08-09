#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "runner/api.h"

int main(int argc, char const *argv[])
{
    char const* home = "dist/taskshome";
    char* command = (char*) argv[1];

    if (strcmp(command, "run") == 0)
    {
        int total_length = 0;
        for (int i = 2; i < argc; i++)
        {
            if (i > 2)
            {
                total_length += 1;
            }

            total_length += strlen(argv[i]);
        }

        char command[total_length + 1];
        command[0] = 0;
        for (int i = 2; i < argc; i++)
        {
            if (i > 2)
            {
                strcat(command, " ");
            }
            strcat(command, argv[i]);
        }

        int task_id = run(
            home,
            command,
            "."
        );
        printf("%d\n", task_id);
    }
    else if (strcmp(command, "stop") == 0)
    {
        char *_;
        int task_id = strtol(argv[2], &_, 10);
        stop(home, task_id);
    }
    else if (strcmp(command, "status") == 0)
    {
        char *_;
        int task_id = strtol(argv[2], &_, 10);
        TaskStatus task_status = status(home, task_id);
        switch (task_status)
        {
            case RUNNING:
                printf("running\n");
            break;

            case STOPPED:
                printf("stopped\n");
            break;
        }
    }
    else if (strcmp(command, "logs") == 0)
    {
        char *_;
        int task_id = strtol(argv[2], &_, 10);
        char path[100];
        logs(home, task_id, path);
        printf("%s\n", path);
    }
    
    return 0;
}
