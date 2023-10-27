#include "main.h"

char *builtin_str[] = {
    "cd",
    "exit",
    "quit",
    "export",
    "echo",
    "pwd",
    "|"
};

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_exit,
    &sh_exit,
    &sh_export,
    &sh_echo,
    &sh_pwd,
    &sh_pipe
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
            char *token = argv[i];
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
            if (token[0] == '$' && token[1] != '\0') {
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



int sh_pipe(char **argv) { // pipes functionality
    int num_commands = 1;
    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0) {
            num_commands++;
        }
    }

    int pipes[num_commands - 1][2];
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    int command_index = 0;
    int argv_index = 0;
    pid_t pid;
    while (command_index < num_commands) {
        char *command_argv[100];  // adjust size as necessary
        int command_argc = 0;
        while (argv[argv_index] != NULL && strcmp(argv[argv_index], "|") != 0) {
            command_argv[command_argc++] = argv[argv_index++];
        }
        command_argv[command_argc] = NULL;
        argv_index++;  // skip the pipe symbol for the next command

        pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // Child process
            if (command_index > 0) {
                dup2(pipes[command_index - 1][0], STDIN_FILENO);
                close(pipes[command_index - 1][1]);
            }
            if (command_index < num_commands - 1) {
                dup2(pipes[command_index][1], STDOUT_FILENO);
                close(pipes[command_index][0]);
            }
            for (int i = 0; i < num_commands - 1; i++) {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
            execvp(command_argv[0], command_argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        command_index++;
    }

    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }

    return 0;
}
