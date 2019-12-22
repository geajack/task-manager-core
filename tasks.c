#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "tasks.h"

int run(char* command, char* arguments[], int n_arguments, char* cwd)
{
    int process_id = fork();

    if (process_id > 1)
    {
        return process_id;
    }
    else if (process_id == 0)
    {
        int output_file;
        int error_file;
        {
            char output_file_name[10+7];
            char error_file_name[10+7];
            sprintf(output_file_name, "%d.stdout", process_id);
            sprintf(error_file_name, "%d.stderr", process_id);
            const int PERMISSIONS = 
                S_IRUSR | S_IRGRP | S_IROTH |
                S_IWUSR | S_IRGRP | S_IWOTH
            ;
            output_file = open(output_file_name, O_RDWR | O_CREAT, PERMISSIONS);
            error_file = open(error_file_name, O_RDWR | O_CREAT, PERMISSIONS);
        }
        dup2(output_file, 1);
        dup2(error_file, 2);
        close(output_file);
        close(error_file);

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
    else
    {
        return -1;
    }
}

void stop(int task_id)
{

}

TaskStatus status(int task_id)
{
}

char* logs(int task_id)
{

}