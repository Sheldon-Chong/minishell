#include "minishell.h"



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
	if (current_chunk->heredoc_buffer != NULL)
	{
		printf("heredoc detected\n");
		int fd[2];
		pipe(fd);
		ft_putstr_fd(current_chunk->heredoc_buffer, fd[1]);
		close(fd[1]);
		executor->cmd_in = fd[0];
		return ;
	}
	executor->cmd_in = executor->pipefd[0]; // the output of the current command is the input for the next command
}

// Function to run a list of commands with piping
void executor(char **env, t_token_info *token_info)
{
	t_token *chunk_list = token_info->token_chunks;
	t_executor *executor = executor_init();

	set_fd_in(chunk_list, executor);
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
