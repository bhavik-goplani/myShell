#include "main.h"

char *get_location(char *command) {
    char *path, *path_copy, *token, *file_path;
    int command_len, token_len;
    struct stat buffer;

    path = getenv("PATH");

    if (path) {
        path_copy = strdup(path);
        command_len = strlen(command);

        token = strtok(path_copy, ":");

        while(token != NULL) {
            token_len = strlen(token);
            file_path = malloc(token_len + command_len + 2);

            strcpy(file_path, token);
            strcat(file_path, "/"); 
            strcat(file_path, command);
            strcat(file_path, "\0"); 

            if (stat(file_path, &buffer) == 0) {
                free(path_copy);
                return (file_path);
            }
            else {
                free(file_path);
                token = strtok(NULL, ":");
            }
        }
        free(path_copy);

        if (stat(command, &buffer) == 0) {
            return (command);
        }
        return (NULL);
    }
    return (NULL);
}
