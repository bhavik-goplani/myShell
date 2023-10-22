#include "main.h"

char *builtin_str[] = {
    "cd",
    "exit",
    "quit",
    "export",
    "echo"
};

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_exit,
    &sh_exit,
    &sh_export,
    &sh_echo
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

int sh_export(char **argv)
{
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
                if (value[0] == '$') {
                    value++;
                    char *result = helper_env_path(value);
                    setenv(name, result, 1);
                    free(result);
                }
                else if (setenv(name, value, 1) == -1) {
                    perror("ErrorExportingVariable:");
                }
            }
        }
    }
    return (0);
}

int sh_echo(char **argv)
{
    if (argv[1] == NULL) {
        printf("\n");
    }
    else {
        for (int i = 1; argv[i] != NULL; i++) {
            char *token = strdup(argv[i]);
            if (token[0] == '$') {
                token++;
                char *value = helper_env_path(token);
                printf("%s ", value);
                free(value);
            }
            else {
                printf("%s ", argv[i]);
            }
        }
        printf("\n");
    }
    return (0);
}

char *helper_env_path(char *token) {
    char *path = NULL;
    char *value = NULL;

    for(int i = 0; i< (int)strlen(token); i++) {
        if (token[i] == '/') {
            path = token + i;
            break;
        }
    }
    char *environ = strtok(token, "/");
    value = getenv(environ);
    if (value != NULL) {
        char *result = malloc(strlen(value) + strlen(path) + 1);
        strcpy(result, value);
        strcat(result, path);
        strcat(result, "\0");
        return (result);
    }
    return (path);
}