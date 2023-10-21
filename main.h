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
void free_argv(char **argv);
int sh_exit(char **argv);
int sh_cd(char **argv);
int sh_num_builtins();
extern char *builtin_str[];
extern int (*builtin_func[]) (char **);
