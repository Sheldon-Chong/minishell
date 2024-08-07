/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:41 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:41 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_error(char *error_name)
{
	perror("pipe");
	exit(EXIT_FAILURE);
}

void	run_cmd(t_token *chunk, t_token_info *token_info,
			int cmd_in_fd, int cmd_out)
{
	if (str_in_arr(chunk->start->word, "exit"))
		ft_exit(chunk->tokens, token_info);
	else if (str_in_arr(chunk->start->word, "cd"))
	{
		if (chdir(chunk->tokens[1]) == -1)
			perror("chdir");
	}
	else if (str_in_arr(chunk->start->word, "unset"))
		unset_env(chunk->tokens + 1,
			&(token_info->env_data->env_list), token_info);
	else if (str_in_arr(chunk->start->word, "export") && chunk->tokens[1])
		ft_export(chunk->tokens, token_info);
	else
		exec_cmd(chunk->tokens, token_info, cmd_in_fd, cmd_out);
}


void	set_outf(t_token *chunk_list, t_executor *exe, int *pipefd)
{
	int	fd;

	if (chunk_list->outfile)
	{
		if (chunk_list->outfile_mode == 'a')
			fd = open(chunk_list->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else
			fd = open(chunk_list->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		exe->cmd_out = fd;
	}
	else if (chunk_list->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		exe->cmd_out = pipefd[1];
	}
	else
		exe->cmd_out = STDOUT_FILENO;
}


void heredoc(int *heredoc_fd, t_token_info *token_info, t_token *chunk_list)
{
	if (pipe(heredoc_fd) == -1)
		exit_error("open");
	ft_putstr_fd(chunk_list->heredoc_buffer, heredoc_fd[1]);
	close(heredoc_fd[1]);
	token_info->executor->cmd_in = heredoc_fd[0];
}

void	executor(char **env, t_token_info *token_info)
{
	t_token		*chunk_list;
	int			pipefd[2];
	int			file_fd_in;
	int			file_fd_out;
	pid_t		pid;
	int			heredoc_fd[2];

	chunk_list = token_info->token_chunks;
	token_info->executor = executor_init();
	while (chunk_list)
	{
		set_outf(chunk_list, token_info->executor, pipefd);
		if (chunk_list->infile)
		{
			file_fd_in = open(chunk_list->infile, O_RDONLY);
			if (file_fd_in == -1)
				exit_error("open infile");
			token_info->executor->cmd_in = file_fd_in;
		}
		else if (chunk_list->heredoc_buffer != NULL)
		{
			heredoc(heredoc_fd, token_info, chunk_list);
		}
		if (!str_in_arr(chunk_list->tokens[0], "exit,unset,export,cd")
			|| (!ft_strcmp(chunk_list->tokens[0], "export")
				&& !chunk_list->tokens[1]))
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if (pid == 0)
			{
				signal(SIGTERM, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				if (token_info->executor->cmd_in != STDIN_FILENO)
				{
					dup2(token_info->executor->cmd_in, STDIN_FILENO);
					close(token_info->executor->cmd_in);
				}
				if (token_info->executor->cmd_out != STDOUT_FILENO)
				{
					dup2(token_info->executor->cmd_out, STDOUT_FILENO);
					close(token_info->executor->cmd_out);
				}
				if (chunk_list->next)
					close(pipefd[0]);
				run_cmd(chunk_list, token_info, STDIN_FILENO, STDOUT_FILENO);
				
				exit(g_exit_status);
			}
			else
			{
				if (token_info->executor->cmd_in != STDIN_FILENO)
					close(token_info->executor->cmd_in);
				if (token_info->executor->cmd_out != STDOUT_FILENO)
					close(token_info->executor->cmd_out);
				if (chunk_list->next)
				{
					token_info->executor->cmd_in = pipefd[0];
					close(pipefd[1]);
				}
			}
		}
		else
		{
			if (!strcmp(chunk_list->tokens[0], "exit"))
			{
				ft_exit(chunk_list->tokens, token_info);
			}
			else if (!strcmp(chunk_list->tokens[0], "cd"))
			{
				if (chdir(chunk_list->tokens[1]) == -1)
					perror("chdir");
			}
			else if (!strcmp(chunk_list->tokens[0], "unset"))
				unset_env(chunk_list->tokens + 1,
					&(token_info->env_data->env_list), token_info);
			else if (!strcmp(chunk_list->tokens[0], "export"))
				ft_export(chunk_list->tokens, token_info);
		}
		chunk_list = chunk_list->next;
	}
	while (wait(&g_exit_status) > 0)
	{
		g_exit_status = WEXITSTATUS(g_exit_status);
	}
	free(token_info->executor);
}
