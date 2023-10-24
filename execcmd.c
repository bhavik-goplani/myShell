#include "main.h"

void execcmd(char **argv){

    char *command = NULL, *command_location = NULL;

    if (argv) {
        command = argv[0];
        command_location = get_location(command);

        for (int i = 1; argv[i] != NULL; i++) {
            char *token = argv[i];
            if (token[0] == '$') {
                token++;
                char *result = helper_env_path(token);
                if (result != NULL) {
                    free (argv[i]);
                    argv[i] = malloc(strlen(result) + 1);
                    strcpy(argv[i], result);
                    free(result);
                }
            }
        }
        if (execve(command_location, argv, NULL) == -1) {
            free(command_location);
            char error_message[100];
            sprintf(error_message, "%s: command not found", command);
            perror(error_message);
        }
        else { 
            free(command_location);
        }
    }
}
