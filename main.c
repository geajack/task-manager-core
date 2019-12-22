#include "tasks.h"

int main(int argc, char const *argv[])
{
    char* command = (char*) argv[1];
    run(
        command,
        (char**) &argv[2],
        argc - 1,
        "."
    );
    return 0;
}
