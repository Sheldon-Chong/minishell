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
	struct sigaction sa_ignore, sa_default, sa_old_int, sa_old_quit;

	// Set up the ignore action
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	// Set up the default action
	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	// Save the old signal handlers and set new ones
	sigaction(SIGINT, &sa_ignore, &sa_old_int);
	sigaction(SIGQUIT, &sa_ignore, &sa_old_quit);
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
	free(buffer);
	// Restore the old signal handlers
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	return (str);
}
