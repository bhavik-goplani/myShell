#include "main.h"

char *builtin_str[] = {
    "cd",
    "exit",
    "quit",
    "export",
    "echo",
    "pwd"
};

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_exit,
    &sh_exit,
    &sh_export,
    &sh_echo,
    &sh_pwd
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
        for (int i = 1; argv[i] != NULL; i++) {
            char *token = strdup(argv[i]);
            if (token[0] == '$') {
                token++;
                char *result = helper_env_path(token);
                if (chdir(result) == -1) {
                    perror("ErrorChangingDirectory:");
                }
                else {
                    setenv("PWD", result, 1);
                }
                if (result != NULL) {
                    free(result);
                }
            }
            else if (chdir(argv[i]) == -1) {
                perror("ErrorChangingDirectory:");
            }
            else {
                char *cwd = getcwd(NULL, 0);
                setenv("PWD", cwd, 1);
                free(cwd);
            }
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
        argv = helper_remove_quotes(argv);
        for (int i = 1; argv[i] != NULL; i++) {
            char *token = argv[i];
            if (token[0] == '$') {
                token++;
                char *value = helper_env_path(token);
                printf("%s ", value);
                if (value != NULL){
                    free(value);
                }
            }
            else {
                printf("%s ", argv[i]);
            }
        }
        printf("\n");
    }
    return (0);
}

int sh_pwd()
{
    char *cwd = getcwd(NULL, 0);
    printf("%s\n", cwd);
    free(cwd); 
    return (0);
}

char *helper_env_path(char *token) {
    char *path = NULL;
    char *value = NULL;
    
    char *token_copy = strdup(token);
    if (token_copy == NULL) {
        return NULL;
    }

    for (int i = 0; i < (int)strlen(token); i++) {
        if (token[i] == '/') {
            path = token + i;
            break;
        }
    }
    
    char *environ = strtok(token_copy, "/");
    value = getenv(environ);
    
    if (value != NULL && path != NULL) {
        char *result = (char *)malloc(strlen(value) + strlen(path) + 1);
        if (result == NULL) {
            free(token_copy);
            return NULL;
        }
        strcpy(result, value);
        strcat(result, path);
        strcat(result, "\0");
        free(token_copy);  
        return result;
    } else if (value != NULL) {
        char *result = (char *)malloc(strlen(value) + 1);
        if (result == NULL) {
            free(token_copy);
            return NULL;
        }
        strcpy(result, value);
        strcat(result, "\0");
        free(token_copy);  
        return result;
    } else if (path != NULL) {
        char *result = (char *)malloc(strlen(path) + 1);
        if (result == NULL) {

            free(token_copy);
            return NULL;
        }
        strcpy(result, path);
        strcat(result, "\0");
        free(token_copy);  
        return result;
    } else {
        free(token_copy); 
        return NULL;
    }
}

char **helper_remove_quotes(char **argv) {
    char temp = "\0";
    for (int i = 0; argv[i] != NULL; i++) {
        char *token = strdup(argv[i]);
        
        if (token[0] == '\"' || token[0] == '\'' && (temp == "\0")) {
            temp = token[0];
            token = token + 1;
            memmove(argv[i], token, strlen(token));
        }
        if (token[strlen(token) - 1] == '\"' || token[strlen(token) - 1] == '\'' && (temp == token[strlen(token) - 1])) {
            // argv[i][strlen(token) - 1] = '\0'; 
            token[strlen(token) - 1] = '\0';
            memmove(argv[i], token, strlen(token));
        }
        printf("%s\n", argv[i]);
        free(token); 
    }
    return argv;
}