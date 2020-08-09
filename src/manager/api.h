typedef struct LaunchConfiguration
{
    char* command;
    char* cwd;
} LaunchConfiguration;

typedef struct StartedTask
{
    LaunchConfiguration launch_configuration;
    char const *label;
    int runner_id;
} StartedTask;

typedef struct StartedTasksList
{
    unsigned int count;
    StartedTask *tasks;
} StartedTasksList;

int get_running_tasks(char const *home, StartedTasksList *tasks);
int start(char const *home, LaunchConfiguration *config);

void started_tasks_list_destroy(StartedTasksList *list);