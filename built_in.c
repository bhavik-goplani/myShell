#include "main.h"

char *builtin_str[] = {
    "cd",
    "exit",
    "quit",
    "export"
};

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_exit,
    &sh_exit,
    &sh_export
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

int sh_export(char **argv)
{
    if (strcmp(argv[0], "export") == 0) {
        if (argv[1] == NULL) {
            printf("No arguments given to export\n");
        }
        else {

            for (int i = 1; argv[i]!= NULL; i++) {
                char *name = strtok(argv[i], "=");
                char *value = strtok(NULL, "=");

                if (name == NULL || value == NULL) {
                    printf("Invalid argument given to export\n");
                    return (0);
                }
                else {
                    if (setenv(name, value, 1) == -1) {
                        perror("ErrorExportingVariable:");
                    }
                }
            }
        }
        return (0);
    }
    return (1);
}