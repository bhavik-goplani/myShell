#include "main.h"

void execcmd(char **argv){

    char *command = NULL, *command_location = NULL;

    if (argv) {
        command = argv[0];
        command_location = get_location(command);

        if (execve(command_location, argv, NULL) == -1) {
            perror("ErrorLaunchingProcess:");
        }
    }
}