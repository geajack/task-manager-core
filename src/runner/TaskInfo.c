static const int PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

typedef struct TaskInfo
{
    int process_id;
    int start_time;
} TaskInfo;

void create_task_info_for_process(TaskInfo *info, int process_id)
{
    struct stat file_details;
    {
        char process_file_path[20];
        sprintf(process_file_path, "/proc/%d", process_id);
        lstat(process_file_path, &file_details);
    }
    int start_time = file_details.st_ctim.tv_nsec;

    info->process_id = process_id;
    info->start_time = start_time;
}

int task_info_from_file(TaskInfo *info, char const *filepath)
{
    FILE *file = fopen(filepath, "rb");
    fread(&info->process_id, 1, sizeof(info->process_id), file);
    fread(&info->start_time, 1, sizeof(info->start_time), file);
    fclose(file);

    return 0;
}

int task_info_to_file(TaskInfo *info, char const *filepath)
{
    int info_file = open(filepath, O_WRONLY | O_CREAT, PERMISSIONS);
    write(info_file, &info->process_id, sizeof(info->process_id));
    write(info_file, &info->start_time, sizeof(info->process_id));
    close(info_file);
}

int task_info_get_process_id(TaskInfo *info)
{
    return info->process_id;
}

int task_info_get_start_time(TaskInfo *info)
{
    return info->start_time;
}

TaskStatus task_info_get_status(TaskInfo *info)
{
    int process_id = task_info_get_process_id(info);
    int start_time = task_info_get_start_time(info);
    
    struct stat file_details;
    int io_error;
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

int destroy_task_info(TaskInfo *info)
{
    return 0;
}