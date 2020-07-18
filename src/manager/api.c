#include "./api.h"
#include "../runner/api.h"

int start(char const *home, TaskConfig *config)
{
    return run(
        home,
        config->command,
        config->arguments,
        config->n_arguments,
        config->cwd
    );
}