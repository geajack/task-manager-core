#include <sys/unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int process = fork();

    if (process > 1)
    {
        int result;
        wait(&result);
    }
    else if (process == 0 )
    {
        char* command = "python3";
        char* arguments[] = {
            command,
            "-m",
            "http.server",
            NULL
        };
        execvp(command, arguments);
    }
    else
    {
        printf("Could not spawn new process!");
    }

    return 0;
}
