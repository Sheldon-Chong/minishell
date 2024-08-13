/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:41 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:41 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child(t_chunk *chunk_list, t_shell_data *shell_data)
{
	signal(SIGTERM, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	set_child_redirections(shell_data);
	run_cmd(chunk_list, shell_data, STDIN_FILENO, STDOUT_FILENO);
	exit(g_exit_status);
}

void	parent(t_chunk *chunk_list, t_shell_data *shell_data)
{
	if (shell_data->executor->cmd_in != STDIN_FILENO)
		close(shell_data->executor->cmd_in);
	if (shell_data->executor->cmd_out != STDOUT_FILENO
		&& shell_data->executor->cmd_out != shell_data->executor->pipefd[1])
		close(shell_data->executor->cmd_out);
	if (chunk_list->next)
	{
		shell_data->executor->cmd_in = shell_data->executor->pipefd[0];
		close(shell_data->executor->pipefd[1]);
	}
	else
	{
		close(shell_data->executor->pipefd[1]);
		close(shell_data->executor->pipefd[0]);
	}
}

void	set_child_redirections(t_shell_data *shell_data)
{
	if (shell_data->executor->cmd_in != STDIN_FILENO)
	{
		dup2(shell_data->executor->cmd_in, STDIN_FILENO);
		close(shell_data->executor->cmd_in);
	}
	if (shell_data->executor->cmd_out != STDOUT_FILENO)
	{
		dup2(shell_data->executor->cmd_out, STDOUT_FILENO);
		if (shell_data->executor->cmd_out != shell_data->executor->pipefd[1])
			close(shell_data->executor->cmd_out);
	}
	close(shell_data->executor->pipefd[1]);
	close(shell_data->executor->pipefd[0]);
}

void	dup_fd_for_child(int cmd_in_fd, int cmd_out)
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
}
