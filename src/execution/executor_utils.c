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

t_chunk	*get_chunk_start(t_token *start, int pos)
{
	int	i;

	i = 0;
	while (i < pos)
	{
		start = start->next;
		i++;
	}
	if (!start)
		return (NULL);
	return (start);
}

void	set_outf(t_token *chunk_list, t_executor *exe)
{
	int	fd;

	if (pipe(exe->pipefd) == -1)
		exit_error("pipe");
	if (chunk_list->outfile)
	{
		if (chunk_list->outfile_mode == 'a')
			fd = open(chunk_list->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else
			fd = open(chunk_list->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		exe->cmd_out = fd;
	}
	else if (chunk_list->next)
		exe->cmd_out = exe->pipefd[1];
	else
		exe->cmd_out = STDOUT_FILENO;
}

void	set_inf(t_executor *executor, t_chunk *chunk_list,
			t_shell_data *shell_data)
{
	int		file_fd_in;
	int		pipefd[2];

	if (chunk_list->infile)
	{
		file_fd_in = open(chunk_list->infile, O_RDONLY);
		if (file_fd_in == -1)
			exit_error("open infile");
		executor->cmd_in = file_fd_in;
	}
	else if (chunk_list->heredoc_buffer != NULL)
	{
		if (pipe(pipefd) == -1)
			exit_error("pipe");
		ft_putstr_fd(chunk_list->heredoc_buffer, pipefd[1]);
		close(pipefd[1]);
		executor->cmd_in = pipefd[0];
	}
	else if (shell_data->start_pos != 0)
	{
		if (pipe(pipefd) == -1)
			exit_error("pipe");
		executor->cmd_in = pipefd[0];
		close(pipefd[1]);
	}
}
