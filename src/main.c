#include <stdio.h>

#include "tasks.h"

int main(int argc, char const *argv[])
{
    char* command = (char*) argv[1];
    int task_id = run(
        "dist/taskshome",
        command,
        (char**) &argv[2],
        argc - 1,
        "."
    );
    printf("%d\n", task_id);
    return 0;
}
