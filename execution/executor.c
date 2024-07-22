#include "minishell.h"

int exit_error(char *error_name)
{
	perror("pipe");
	exit(EXIT_FAILURE);
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

void set_fd_out(t_token *current_chunk, t_executor *executor)
{
	int	file_fd;

	if (current_chunk->next)
	{
		if (pipe(executor->pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		executor->cmd_out = executor->pipefd[1];
	}
	else
		executor->cmd_out = STDOUT_FILENO; 
	
	if (current_chunk->outfile == NULL)
		return ;
	if (current_chunk->outfile_mode == 'a')
		file_fd = open(current_chunk->outfile, O_WRONLY | O_APPEND);
	else
		file_fd = open(current_chunk->outfile, O_WRONLY | O_TRUNC);
	executor->cmd_out = file_fd; 
}


// Set the stdin of the current command
void set_fd_in(t_token *current_chunk, t_executor *executor)
{
	if (current_chunk->heredoc_buffer != NULL)
	{
		int fd[2];
		if (pipe(fd) == -1)
			exit_error("pipe");
		ft_putstr_fd(current_chunk->heredoc_buffer, fd[1]);
		close(fd[1]);
		executor->cmd_in = fd[0];
	}
	else if (current_chunk->infile != NULL)
	{
		int file_fd = open(current_chunk->infile, O_RDONLY);
		if (file_fd == -1)
			exit_error("open");
		executor->cmd_in = file_fd;
	}
	else
		executor->cmd_in = executor->pipefd[0]; // the output of the previous command is the input for the next command
}

// Function to run a list of commands with piping
void executor(char **env, t_token_info *token_info)
{
	int			prev_pipe_in;
	t_token		*chunk_list;
	t_executor	*executor;

	executor = executor_init();
	chunk_list = token_info->token_chunks;
	prev_pipe_in = STDIN_FILENO;

	while (chunk_list)
	{
		set_fd_in(chunk_list, executor);
		set_fd_out(chunk_list, executor);

		run_cmd(chunk_list, env, token_info, executor->cmd_in, executor->cmd_out);

		if (prev_pipe_in != STDIN_FILENO)
			close(prev_pipe_in);

		if (executor->cmd_out != STDOUT_FILENO)
			close(executor->cmd_out);

		prev_pipe_in = executor->pipefd[0];
		chunk_list = chunk_list->next;
	}
	if (executor->cmd_in != STDIN_FILENO)
		close(executor->cmd_in);
		
	while (wait(&executor->status) > 0)
		nothing();

	free(executor);
}