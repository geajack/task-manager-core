#pragma once

typedef enum TaskStatusCode
{
    RUNNING,
    STOPPED
} TaskStatusCode;

typedef struct
{
    TaskStatusCode code;
    int result;
} TaskStatus;

int run(char const* home, char* command, char* arguments[], int n_arguments, char const* cwd);

void stop(char const* home, int task_id);

TaskStatus* status(int);

char* logs(int);