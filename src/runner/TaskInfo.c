#include "cJSON.h"

static const int PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

typedef struct TaskInfo
{
    cJSON *json;
} TaskInfo;

void create_task_info_for_process(TaskInfo *info, int process_id)
{
    cJSON *json = cJSON_CreateObject();
        
    cJSON *pid_json = cJSON_CreateNumber((double) process_id);
    
    struct stat file_details;
    {
        char process_file_path[20];
        sprintf(process_file_path, "/proc/%d", process_id);
        lstat(process_file_path, &file_details);
    }
    int start_time = file_details.st_ctim.tv_nsec;
    cJSON *start_time_json = cJSON_CreateNumber((double) start_time);

    cJSON_AddItemToObject(json, "pid", pid_json);
    cJSON_AddItemToObject(json, "start_time", start_time_json);

    info->json = json;
}

int task_info_from_file(TaskInfo *info, char const *filepath)
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

int task_info_to_file(TaskInfo *info, char const *filepath)
{
    const char* contents = cJSON_Print(info->json);    
    int info_file = open(filepath, O_RDWR | O_CREAT, PERMISSIONS);
    write(info_file, contents, strlen(contents));
    close(info_file);
    free((void*) contents);
}

int task_info_get_process_id(TaskInfo *info)
{
    return cJSON_GetObjectItem(info->json, "pid")->valueint;
}

int task_info_get_start_time(TaskInfo *info)
{
    return cJSON_GetObjectItem(info->json, "start_time")->valueint;
}

int destroy_task_info(TaskInfo *info)
{
    free(info->json);
    return 0;
}