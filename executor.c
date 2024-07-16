#include "minishell.h"

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

void exec_cmd(char **cmd, char **env, t_token_info *token_info, int cmd_in_fd, int cmd_out)
{
	char *command;
	pid_t pid;

	command = get_path(cmd[0], env);
	pid = fork();
	if (pid != 0) // parent
	{
		if (cmd_in_fd != STDIN_FILENO)
			close(cmd_in_fd);
		if (cmd_out != STDOUT_FILENO)
			close(cmd_out);
		waitpid(pid, NULL, 0);
		if (command[0] != '\0')
			free(command);
	}
	else if (pid == 0) //child
	{
		if (cmd_in_fd != STDIN_FILENO)
		{
			dup2(cmd_in_fd, STDIN_FILENO);
			close(cmd_in_fd);
		}
		if (cmd_out != STDOUT_FILENO)
		{
			dup2(cmd_out, STDOUT_FILENO);
			close(cmd_out);
		}
		if (access(command, F_OK) == 0)
			execve((const char *)command, (char *const *)cmd, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void run_cmd(t_token *chunk, char **env, t_token_info *token_info, int cmd_in_fd, int cmd_out)
{
	if (str_in_arr(chunk->start->word, BASH_CMDS))
		bash_cmd(env, token_info);
	else
		exec_cmd(chunk->tokens, env, token_info, cmd_in_fd, cmd_out);
}

t_executor *executor_init()
{
	t_executor *executor = malloc(sizeof(t_executor));
	executor->cmd_in = STDIN_FILENO;
	executor->cmd_out = STDOUT_FILENO;
	executor->status = 0;
	return executor;
}

// set the stdout of the current command
void set_fd_out(t_token *current_chunk, t_executor *executor)
{
	if (current_chunk->next)
	{
		if (pipe(executor->pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}
	else
		executor->pipefd[1] = STDOUT_FILENO; // Last command writes to stdout
	executor->cmd_out = executor->pipefd[1];
}

// set the stdin of the current command
void set_fd_in(t_token *current_chunk, t_executor *executor)
{
	executor->cmd_in = executor->pipefd[0]; // the output of the current command is the input for the next command
}

// Function to run a list of commands with piping
void executor(char **env, t_token_info *token_info)
{
	t_token *chunk_list = token_info->token_chunks;
	t_executor *executor = executor_init();

	while (chunk_list)
	{
		set_fd_out(chunk_list, executor); //set stdout of current command
		run_cmd(chunk_list, env, token_info, executor->cmd_in, executor->cmd_out); //run the command
		if (executor->cmd_in != STDIN_FILENO)
			close(executor->cmd_in);
		if (executor->cmd_out != STDOUT_FILENO)
			close(executor->cmd_out);
		set_fd_in(chunk_list, executor); //set the stdin of the next command
		chunk_list = chunk_list->next;
	}

	if (executor->cmd_in != STDIN_FILENO)
		close(executor->cmd_in);

	while (wait(&executor->status) > 0)
		nothing();
}
