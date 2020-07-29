typedef struct LaunchConfiguration
{
    char* command;
    char* cwd;
} LaunchConfiguration;

typedef struct StartedTask
{
    LaunchConfiguration launch_configuration;
    char const *label;
} StartedTask;

typedef struct StartedTasksList
{
    unsigned int count;
    StartedTask *tasks;
} StartedTasksList;

int get_running_tasks(char const *home, StartedTasksList *tasks);
int start(char const *home, LaunchConfiguration *config);