#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>


void execcmd(char **argv);
char *get_location(char *command);
char **parse_command(char *lineptr, char **argv);