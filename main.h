#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h>


void execcmd(char **argv);
char *get_location(char *command);

void remove_comments(char *input);
char *helper_env_path(char *token);
char **helper_remove_quotes(char **argv);

char **parse_command(char *lineptr, char **argv);
void free_argv(char **argv);
int sh_exit();
int sh_cd(char **argv);
int sh_export(char **argv);
int sh_echo(char **argv);
int sh_pwd();
int sh_num_builtins();
int sh_launch(char **argv);
int sh_execute(char **argv);
extern char *builtin_str[];
extern int (*builtin_func[]) (char **);

int redirection_check(char **argv);
int redirection(char **argv);
int handle_redirection(char **argv, int state, int *index);
int execute_redirection(char **argv);

int sh_pipe(char **argv);
