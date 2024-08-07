/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 14:37:11 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 18:22:21 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_executor	*executor_init(void)
{
	t_executor	*executor;

	executor = malloc(sizeof(t_executor));
	executor->cmd_in = STDIN_FILENO;
	executor->cmd_out = STDOUT_FILENO;
	executor->status = 0;
	return (executor);
}

void	set_outfile(t_token *chunk_list, t_executor *exe, int *pipefd)
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

void	set_infile(t_token *chunk_list, t_executor *exe)
{
	int	fd;
	int	heredoc_fd[2];

	if (chunk_list->infile)
	{
		fd = open(chunk_list->infile, O_RDONLY);
		if (fd == -1)
		{
			perror("open infile");
			exit(EXIT_FAILURE);
		}
		exe->cmd_in = fd;
	}
	else if (chunk_list->heredoc_buffer != NULL)
	{
		if (pipe(heredoc_fd) == -1)
			exit_error("open");
		ft_putstr_fd(chunk_list->heredoc_buffer, heredoc_fd[1]);
		close(heredoc_fd[1]);
		exe->cmd_in = heredoc_fd[0];
	}
}

void	check_file(int cmd_in, int cmd_out)
{
	if (cmd_in == -1)
		perror("open infile");
	if (cmd_out == -1)
		perror("open outfile");
	exit(EXIT_FAILURE);
}
