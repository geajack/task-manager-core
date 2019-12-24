#pragma once

typedef enum TaskStatus
{
    RUNNING,
    STOPPED
} TaskStatus;

int run(char const* home, char* command, char* arguments[], int n_arguments, char const* cwd);

void stop(char const* home, int task_id);

TaskStatus status(char const* home, int task_id);

void logs(char const *home, int task_id, char *buffer);