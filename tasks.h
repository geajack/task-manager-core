typedef enum TaskStatusCode
{
    RUNNING,
    STOPPED
} TaskStatusCode;

typedef struct
{
    TaskStatusCode code;
    int result;
} TaskStatus;

int run(char* home, char* command, char* arguments[], int n_arguments, char* cwd);

void stop(int);

TaskStatus status(int);

char* logs(int);