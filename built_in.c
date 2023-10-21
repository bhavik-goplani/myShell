#include "main.h"

int sh_exit(char **argv)
{
    if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0) {
        return (0);
    }
    return (1);
}

int sh_cd(char **argv)
{
    if (strcmp(argv[0], "cd") == 0) {
        if (argv[1] == NULL) {
            chdir(getenv("HOME"));
        }
        else {
            if (chdir(argv[1]) == -1) {
                perror("ErrorChangingDirectory:");
            }
        }
        return (0);
    }
    return (1);
}