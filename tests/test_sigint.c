#include "runner/api.h"

void main()
{
    run("dist/taskshome", "python3 -m http.server", "/home/jack");
    while (1) {}
}