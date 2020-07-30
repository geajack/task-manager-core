 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <sys/dir.h>

int repository_get_next_id(const char* home)
{
    int next_task_id = 0;
    DIR* tasks_directory = opendir(home);
    if (tasks_directory)
    {
        struct dirent *file;
        while ((file = readdir(tasks_directory)) != 0)
        {
            if (file->d_type == 8)
            {
                char* _;
                int task_id = strtol(file->d_name, &_, 10);

                if (task_id >= next_task_id)
                {
                    next_task_id = task_id + 1;
                }
            }
        }
        closedir(tasks_directory);
    }
    return next_task_id;
}

char* repository_get_file_path_for_id(const char* home, int id)
{
    int path_length = strlen(home) + 11;
    char *path = malloc(path_length);
    sprintf(path, "%s/%d", home, id);
    return path;
}