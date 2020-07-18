typedef int TaskID;

typedef int FavoriteID;

typedef struct FavoriteHeader
{

} FavoriteHeader;

typedef struct TaskHeader
{

} TaskHeader;

typedef struct TaskDetails
{

} TaskDetails;

typedef struct FavoriteDetails
{

} FavoriteDetails;

typedef struct TaskRecord
{

} TaskRecord;

typedef struct FavoriteRecord
{

} FavoriteRecord;

typedef struct FavoritesList
{
    unsigned int count;
    FavoriteHeader *favorites;
} FavoritesList;

typedef struct TasksList
{
    unsigned int count;
    TaskHeader *tasks;
} TasksList;

typedef struct TaskConfig
{
    char* command;
    char** arguments;
    int n_arguments;
    char* cwd;
} TaskConfig;

int get_favorites(char const *home, FavoritesList *favorites);
int get_running_tasks(char const *home, TasksList *tasks);
int get_favorite_details(char const *home, FavoriteID id, FavoriteDetails *favorite);
int get_task_details(char const *home, TaskID id, TaskDetails *details);
int start(char const *home, TaskConfig *config);
int start_favorite(char const *home, FavoriteID id);
int end(char const *home, TaskID id);
int delete_favorite(char const *home,  FavoriteID id);
int create_favorite(char const *home, FavoriteRecord *favorite);
int set_favorite(char const *home,  FavoriteID id, FavoriteRecord *favorite);