#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>

volatile sig_atomic_t g_exit_status;

void ctrl_c_function(int signum)
{
    write(2, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_sigint(int signum)
{
    // Do nothing, just catch the signal to prevent termination
}

int main(int ac, char **av, char **env)
{
    char *user_input;
    char *prompt = "minishell> ";
    char *cat_args[] = {"/usr/bin/cat", NULL};

    signal(SIGINT, ctrl_c_function);
    signal(SIGQUIT, SIG_IGN);

    while (1)
    {
        user_input = readline(prompt);
        if (!user_input)
            break;

        if (strlen(user_input) > 0)
        {
            add_history(user_input);
            pid_t pid = fork();
            if (pid == 0)
            {
                // Child process
                signal(SIGINT, SIG_DFL); // Reset SIGINT to default behavior
                execve(cat_args[0], cat_args, env);
                perror("execve");
                exit(EXIT_FAILURE);
            }
            else if (pid > 0)
            {
                // Parent process
                signal(SIGINT, handle_sigint); // Ignore SIGINT while waiting for child
                int status;
                waitpid(pid, &status, 0);
                signal(SIGINT, ctrl_c_function); // Restore SIGINT handler
            }
            else
            {
                perror("fork");
            }
        }
        free(user_input);
    }
    return 0;
}