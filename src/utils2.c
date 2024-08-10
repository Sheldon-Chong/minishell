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

char	*ft_strndup(const char *str, size_t n)
{
	size_t	i;
	char	*output;

	output = malloc(sizeof(char) * n + 1);
	if (output == NULL)
		return (NULL);
	i = -1;
	while (++i < n)
		output[i] = str[i];
	output[i] = '\0';
	return (output);
}

int	add_substr_to_toklist(const char *str, int start,
		int len, t_token **tokens)
{
	char	*string;

	string = ft_substr(str, start, len);
	if (string[0])
		append(tok(ft_strdup(string), 0), tokens);
	free(string);
	return (start + len);
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
