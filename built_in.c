#include "main.h"

char *builtin_str[] = {
    "cd",
    "exit",
    "quit",
    "export",
    "echo",
    "pwd",
    "|",
    "jobs"
};

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_exit,
    &sh_exit,
    &sh_export,
    &sh_echo,
    &sh_pwd,
    &sh_pipe,
    &sh_jobs
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


int sh_pipe(char **argv) {
    int num_pipes = 0;
    for (int i = 0; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "|") == 0) {
            num_pipes++;
        }
    }

    int pipes[num_pipes][2];
    for (int i = 0; i < num_pipes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    int command_start = 0;
    for (int i = 0; i <= num_pipes; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // If not first command, redirect stdin from previous pipe
            if (i != 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
                close(pipes[i - 1][1]);
            }
            // If not last command, redirect stdout to next pipe
            if (i != num_pipes) {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]);
            }

            // Close all other pipes
            for (int j = 0; j < num_pipes; j++) {
                if (j != i - 1) close(pipes[j][0]);
                if (j != i) close(pipes[j][1]);
            }

            // Handle redirections before executing the command
            int redirection_type = redirection_check(argv);
            if (redirection_type != -1) {
                if (redirection(argv) == -1) {
                    perror("redirection error");
                    exit(EXIT_FAILURE);
                }
            }

            // Execute the command
            char *cmd[100]; // adjust size as necessary
            int cmd_index = 0;
            for (int j = command_start; argv[j] && strcmp(argv[j], "|") != 0; j++) {
                cmd[cmd_index++] = argv[j];
            }
            cmd[cmd_index] = NULL;
            bool builtin = false;
            for (int i = 0; i < sh_num_builtins(); i++) {
                if (strcmp(cmd[0], builtin_str[i]) == 0) {
                    (*builtin_func[i])(cmd);
                    builtin = true;
                }
            }
            if (!builtin) {
                execcmd(cmd);
            }
            perror("execvp");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
            return 1;
        }

        // Move to the next command
        while (argv[command_start] && strcmp(argv[command_start], "|") != 0) {
            command_start++;
        }
        command_start++; // skip the pipe symbol
    }

    // Close all pipes in the parent
    for (int i = 0; i < num_pipes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i <= num_pipes; i++) {
        wait(NULL);
    }

    return 0;
}

int sh_jobs(char **argv) {
    (void) argv;
    print_jobs();
    return 1;
}