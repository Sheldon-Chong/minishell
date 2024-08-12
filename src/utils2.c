/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:46:40 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 18:18:28 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_read(t_token *head, t_token *token_chunk,
		t_shell_data *shell_data, int num)
{
	int	e;
	int	file;

	e = 0;
	token_chunk->infile = head->next->word;
	file = open(token_chunk->infile, O_RDONLY);
	if (file == -1 && g_exit_status == 0)
	{
		e = gen_err(ERR_NOFILDIR, head->next->word, 1);
		shell_data->start_pos = num + e;
		return (-1);
	}
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

void	close_fds(int cmd_in_fd, int cmd_out)
{
	if (cmd_in_fd != STDIN_FILENO)
		close(cmd_in_fd);
	if (cmd_out != STDOUT_FILENO)
		close(cmd_out);
}

int	get_shell_opp_type(char *str)
{
	if (!strncmp(str, ">>", 2))
		return (SH_APPEND);
	else if (!strncmp(str, ">", 1))
		return (SH_WRITE);
	else if (!strncmp(str, "<<", 2))
		return (SH_HEREDOC);
	else if (!strncmp(str, "<", 1))
		return (SH_READ);
	else if (!strncmp(str, "|", 1))
		return (SH_PIPE);
	return (-1);
}

int	get_length_of_list(t_token *head)
{
	int	length;

	length = 0;
	while (head)
	{
		length++;
		head = head->next;
	}
	return (length);
}
