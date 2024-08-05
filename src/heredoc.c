/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:06 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:06 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*here_doc_input(char *delimiter)
{
	char			*buffer;
	char			*str;

	write(STDOUT_FILENO, "> ", 2);
	buffer = get_next_line(STDIN_FILENO);
	str = ft_strdup("");

	while (buffer)
	{
		if (!ft_strncmp(delimiter, buffer, ft_strlen(delimiter))
			&& (ft_strlen(delimiter) == ft_strlen(buffer) - 1))
			break ;
		ft_rstrjoin(&str, buffer);
		free(buffer);
		write(STDOUT_FILENO, "> ", 2);
		buffer = get_next_line(STDIN_FILENO);
	}
	return (free(buffer), str);
}