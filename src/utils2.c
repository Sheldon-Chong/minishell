/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:46:40 by jyap              #+#    #+#             */
/*   Updated: 2024/08/13 07:33:32 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
