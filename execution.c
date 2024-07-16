#include "minishell.h"

/*
THIS FILE WILL BE DELETED SOON. 
*/

//to be replaced by Jyap
char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*sub;
	char	*join;
	char	*pathjoin;

	path = getenv("PATH");
	while (path && *path)
	{
		sub = ft_substr(path, 0, (size_t)(ft_strchr(path, ':') - path));
		join = ft_strjoin(sub, "/");
		pathjoin = ft_strjoin(join, cmd);
		if (access(pathjoin, F_OK) == 0)
			return (f(join, f(sub, (void *)pathjoin)));
		f(pathjoin, f(join, f(sub, NULL)));
		path = ft_strchr(path, ':');
		if (path)
			path ++;
	}
	return ("\0");
}

int	bash_cmd(char **env, t_token_info *token_info)
{
	char	**args;
	int		i;

	args = token_info->cmd_start->tokens;
	if ((!ft_strcmp(args[0], "echo")) && args[1] && !ft_strcmp(args[1], "-n"))
	{
		i = 1;
		while (args[++i])
			printf("%s", args[i]);
	}
	if (!ft_strcmp(args[0], "exit"))
	{
		free_TokenList(token_info);
		exit(0);
	}
	if (!ft_strcmp(args[0], "cd"))
		chdir(args[1]);
	if (!ft_strcmp(args[0], "env"))
		print_env(&token_info->global_env, 'e');
	if (!ft_strcmp(args[0], "unset") && args[1])
		unset_env(args[1], &token_info->global_env, token_info);
	if (!ft_strcmp(args[0], "export"))
		export(token_info);
	return (0);
}

void run_cmd(char **cmd, char **env, t_token_info *token_info, int in_fd, int out_fd)
{
	char *command;
	pid_t pid;

	command = get_path(cmd[0], env);
	pid = fork();
	if (pid != 0) // parent
	{
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (out_fd != STDOUT_FILENO)
			close(out_fd);
		waitpid(pid, NULL, 0);
		if (command[0] != '\0')
			free(command);
	}
	else if (pid == 0) //child
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
	int status;

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
		{
			if (in_fd != STDIN_FILENO)
				close(in_fd);
			bash_cmd(env, token_info);
		}
		else
			run_cmd(chunk_list->tokens, env, token_info, in_fd, pipefd[1]);

		if (pipefd[1] != STDOUT_FILENO)
			close(pipefd[1]);
		in_fd = pipefd[0]; // The input for the next command is the output of the current command
		chunk_list = chunk_list->next;
	}

	// Close the last input file descriptor if it's not STDIN
	if (in_fd != STDIN_FILENO)
		close(in_fd);

	// Wait for all child processes to finish
	while (wait(&status) > 0);

	printf("ended\n");
}
