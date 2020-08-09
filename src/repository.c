 #include "repository.h"
 
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #include <sys/dir.h>

void repository_get_all(const char *home, RepositoryEntryList *list)
{
    DIR* tasks_directory = opendir(home);
    int home_path_length = strlen(home);
    list->entries = malloc(sizeof(RepositoryEntry) * 100);
    int i = 0;
    if (tasks_directory)
    {
        struct dirent *file;
        while ((file = readdir(tasks_directory)) != 0)
        {
            if (file->d_type == 8)
            {
                int task_id = strtol(file->d_name, NULL, 10);
                char* path = malloc(home_path_length + 11);
                sprintf(path, "%s/%s", home, file->d_name);
                list->entries[i].id = task_id;
                list->entries[i].path = path;
                i += 1;
            } 
        }
        closedir(tasks_directory);
    }
    list->count = i;
}

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

void repository_create_entry(const char* home, RepositoryEntry *entry)
{
    entry->id = repository_get_next_id(home);
    entry->path = repository_get_file_path_for_id(home, entry->id);
}

void repository_remove_entry(const char* home, int id)
{
    char *path = repository_get_file_path_for_id(home, id);
    remove(path);
    free(path);
}

void repository_entry_list_destroy(RepositoryEntryList *list)
{
    for (int i = 0; i < list->count; i++)
    {
        repository_entry_destroy(&list->entries[i]);
    }
    free(list->entries);
}

void repository_entry_destroy(RepositoryEntry *entry)
{
    free(entry->path);
}