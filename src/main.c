#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tasks.h"

int main(int argc, char const *argv[])
{
    char const* home = "dist/taskshome";
    char* command = (char*) argv[1];

    if (strcmp(command, "run") == 0)
    {
        char* program = (char*) argv[2];
        char** arguments = (char**) &argv[3];
        int n_arguments = argc - 3;
        int task_id = run(
            home,
            program,
            arguments,
            n_arguments,
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
    
    return 0;
}
