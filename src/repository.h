typedef struct RepositoryEntry
{
    int id;
    char* path;
} RepositoryEntry;

typedef struct RepositoryEntryList
{
    RepositoryEntry *entries;
    int count;
} RepositoryEntryList;

void repository_create_entry(const char* home, RepositoryEntry *entry);
void repository_remove_entry(const char* home, int id);
int repository_get_next_id(const char* home);
char* repository_get_file_path_for_id(const char* home, int id);
void repository_get_all(const char* home, RepositoryEntryList *list);