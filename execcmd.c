#include "main.h"

void execcmd(char **argv){

    char *command = NULL, *command_location = NULL;

    if (argv) {
        command = argv[0];
        printf("command: %s\n", command);
        command_location = get_location(command);
        printf("command_location: %s\n", command_location);

        if (execve(command_location, argv, NULL) == -1) {
            perror("ErrorLaunchingProcess:");
        }
    }
}
