/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:53 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:53 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_shell_opp(int shell_operator_id)
{
	if (shell_operator_id >= SH_PIPE && shell_operator_id <= SH_HEREDOC)
		return (true);
	return (false);
}

int	get_shell_opp_type(char *str)
{
	if (!ft_strncmp(str, ">>", 2))
		return (SH_APPEND);
	else if (!ft_strncmp(str, ">", 1))
		return (SH_WRITE);
	else if (!ft_strncmp(str, "<<", 2))
		return (SH_HEREDOC);
	else if (!ft_strncmp(str, "<", 1))
		return (SH_READ);
	else if (!ft_strncmp(str, "|", 1))
		return (SH_PIPE);
	return (-1);
}

bool	pattern_rec(t_token *pattern_start, t_shell_data *shell_data)
{
	if (is_shell_opp(pattern_start->type) && (!pattern_start->next))
		return (syntax_error("newline", shell_data), true);
	if (!pattern_start->next)
		return (false);
	if (is_shell_opp(pattern_start->type))
	{
		if (pattern_start->type == SH_PIPE
			&& pattern_start->next->type >= SH_WRITE
			&& pattern_start->next->type <= SH_HEREDOC)
			return (false);
		if (is_shell_opp(pattern_start->next->type))
			return (syntax_error(pattern_start->next->word, shell_data), true);
	}
	return (false);
}

bool	post_validate(t_shell_data *shell_data)
{
	t_token	*head;

	head = shell_data->token_list;
	while (head && !shell_data->has_error)
	{
		pattern_rec(head, shell_data);
		head = head->next;
	}
	return (true);
}

void	heredoc_child_init(int *fd, int *empty)
{
	close(fd[0]);
	close(fd[1]);
	close(empty[0]);
	close(empty[1]);
	signal(SIGINT, ctrl_c_heredoc);
}
