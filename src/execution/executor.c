#include "minishell.h"

int exit_error(char *error_name)
{
	perror("pipe");
	exit(EXIT_FAILURE);
}

void run_cmd(t_token *chunk, char **env, t_token_info *token_info, int cmd_in_fd, int cmd_out)
{
	if (str_in_arr(chunk->start->word, "exit"))
		ft_exit(chunk->tokens, token_info);
	else if (str_in_arr(chunk->start->word, "cd"))
	{
		if (chdir(chunk->tokens[1]) == -1)
			perror("chdir");
	}
	else if (str_in_arr(chunk->start->word, "unset"))
		unset_env(chunk->tokens + 1, &(token_info->env_data->env_list), token_info);
	else if (str_in_arr(chunk->start->word, "export") && chunk->tokens[1])
		ft_export(chunk->tokens, token_info);
	else
		exec_cmd(chunk->tokens, env, token_info, cmd_in_fd, cmd_out);
}

t_executor *executor_init()
{
	t_executor	*executor;

	executor = malloc(sizeof(t_executor));
	executor->cmd_in = STDIN_FILENO;
	executor->cmd_out = STDOUT_FILENO;
	executor->status = 0;
	return (executor);
}


void executor(char **env, t_token_info *token_info) {
	t_token *chunk_list = token_info->token_chunks;
	t_executor *executor = executor_init();
	int pipefd[2];

	while (chunk_list)
	{
		if (chunk_list->outfile)
		{
			int file_fd;
			if (chunk_list->outfile_mode == 'a')
				file_fd = open(chunk_list->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
			else
				file_fd = open(chunk_list->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (file_fd == -1) {
				perror("open outfile");
				exit(EXIT_FAILURE);
			}
			executor->cmd_out = file_fd;
		}
		else if (chunk_list->next)
		{
			if (pipe(pipefd) == -1) 
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			executor->cmd_out = pipefd[1];
		} 
		else
			executor->cmd_out = STDOUT_FILENO;

		if (chunk_list->infile)
		{
			int file_fd = open(chunk_list->infile, O_RDONLY);
			if (file_fd == -1)
			{
				perror("open infile");
				exit(EXIT_FAILURE);
			}
			executor->cmd_in = file_fd;
		}
		else if (chunk_list->heredoc_buffer != NULL)
		{
			int fd[2];
			if (pipe(fd) == -1)
				exit_error("open");
			ft_putstr_fd(chunk_list->heredoc_buffer, fd[1]);
			close(fd[1]);
			executor->cmd_in = fd[0];
		}

		pid_t pid;
		
		if (!str_in_arr(chunk_list->tokens[0], "exit,unset,export,cd") || (!ft_strcmp(chunk_list->tokens[0], "export") && !chunk_list->tokens[1]))
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if (pid == 0)
			{
				// Child process 
				if (executor->cmd_in != STDIN_FILENO) 
				{
					dup2(executor->cmd_in, STDIN_FILENO);
					close(executor->cmd_in);
				}
				if (executor->cmd_out != STDOUT_FILENO) 
				{
					dup2(executor->cmd_out, STDOUT_FILENO);
					close(executor->cmd_out);
				}
				if (chunk_list->next) {
					close(pipefd[0]);
				}
				run_cmd(chunk_list, env, token_info, STDIN_FILENO, STDOUT_FILENO);
				printf(">> %d\n", g_exit_status);
				exit(g_exit_status);
			} 
			else
			{
				// Parent process
				if (executor->cmd_in != STDIN_FILENO)
					close(executor->cmd_in);
				if (executor->cmd_out != STDOUT_FILENO)
					close(executor->cmd_out);
				if (chunk_list->next)
				{
					executor->cmd_in = pipefd[0];
					close(pipefd[1]);
				}
			}
		}
		else
		{
			if (!strcmp(chunk_list->tokens[0], "exit"))
				ft_exit(chunk_list->tokens, token_info);
			else if (!strcmp(chunk_list->tokens[0], "cd"))
			{
				if (chdir(chunk_list->tokens[1]) == -1)
					perror("chdir");
			}
			else if (!strcmp(chunk_list->tokens[0], "unset"))
				unset_env(chunk_list->tokens + 1, &(token_info->env_data->env_list), token_info);
			else if (!strcmp(chunk_list->tokens[0], "export"))
				ft_export(chunk_list->tokens, token_info);
		}
		chunk_list = chunk_list->next;
	}
	while (wait(&g_exit_status) > 0)
	{
		printf("EXIT STATUS: %d\n", g_exit_status);
	}
	free(executor);
}