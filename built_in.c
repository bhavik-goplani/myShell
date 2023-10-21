#include "main.h"

char *builtin_str[] = {
  "cd",
  "exit",
  "quit"
};

int (*builtin_func[]) (char **) = {
  &sh_cd,
  &sh_exit,
  &sh_exit
};

int sh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int sh_exit()
{
    return (-1);
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