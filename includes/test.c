void run_cmd(char **cmd, char **env, t_token_info *token_info, int in_fd, int out_fd)
{
    char *command;
    pid_t pid;

    command = get_path(cmd[0], env);
    pid = fork();
    if (pid != 0)
    {
        waitpid(pid, NULL, 0);
        if (command[0] != '\0')
            free(command);
    }
    else if (pid == 0)
    {
        if (in_fd != STDIN_FILENO)
        {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO)
        {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        if (access(command, F_OK) == 0)
            execve((const char *)command, (char *const *)cmd, env);
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

// Function to run a list of commands with piping
void run_cmds(char **env, t_token_info *token_info)
{
    t_token *chunk_list = token_info->token_chunks;
    int pipefd[2];
	int in_fd = STDIN_FILENO;

    while (chunk_list)
    {
        if (chunk_list->next)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        else
            pipefd[1] = STDOUT_FILENO; // Last command writes to stdout

        if (str_in_arr(chunk_list->start->word, BASH_CMDS))
            bash_cmd(env, token_info);
        else
            run_cmd(chunk_list->tokens, env, token_info, in_fd, pipefd[1]);

        close(pipefd[1]);
        in_fd = pipefd[0]; // The input for the next command is the output of the current command
        chunk_list = chunk_list->next;
    }
	printf("ended\n");
}