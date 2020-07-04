#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

#include "TaskInfo.h"

const int PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

int task_info_from_file(char const *filepath, TaskInfo *info)
{
    char* contents;
    {
        FILE *file = fopen(filepath, "rb");
        
        int file_length;
        fseek(file, 0L, SEEK_END);
        file_length = ftell(file);
        rewind(file);

        contents = (char*) malloc(file_length);
        fread(contents, 1, file_length, file);

        fclose(file);
    }

    info->json = cJSON_Parse(contents);
    free(contents);

    return 0;
}

int destroy_task_info(TaskInfo *info)
{
    free(info->json);
    return 0;
}

TaskStatus get_status(TaskInfo *info)
{
    int process_id = cJSON_GetObjectItem(info->json, "pid")->valueint;
    int start_time = cJSON_GetObjectItem(info->json, "start_time")->valueint;
    
    struct stat file_details;
    bool io_error;
    {
        char process_file_path[20];
        sprintf(process_file_path, "/proc/%d", process_id);
        io_error = lstat(process_file_path, &file_details);
    }

    if (!io_error)
    {
        if (start_time == file_details.st_ctim.tv_nsec)
        {
            return RUNNING;
        }
    }

    return STOPPED;
}

int get_pid(TaskInfo *info)
{
    int process_id = cJSON_GetObjectItem(info->json, "pid")->valueint;
    return process_id;
}


void create_task_file(char const* home, int task_id, int process_id)
{
    const char* contents;
    {
        cJSON *json = cJSON_CreateObject();
        
        cJSON *pid_json = cJSON_CreateNumber((double) process_id);
        
        int start_time = time(0);
        cJSON *start_time_json = cJSON_CreateNumber((double) start_time);

        cJSON_AddItemToObject(json, "pid", pid_json);
        cJSON_AddItemToObject(json, "start_time", start_time_json);
        contents = cJSON_Print(json);
    }

    int info_file;
    {
        const int prefix_length = strlen(home) + 7;
        const int n_digits = 10;
        char info_file_path[prefix_length + n_digits];
        sprintf(info_file_path, "%s/tasks/%d", home, task_id);
        info_file = open(info_file_path, O_RDWR | O_CREAT, PERMISSIONS);
    }

    write(info_file, contents, strlen(contents));
    close(info_file);
    free((void*) contents);
}