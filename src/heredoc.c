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

void	*heredoc_input_sub(char *delimiter, char *str)
{
	char	*buffer;

	buffer = get_next_line(STDIN_FILENO);
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
	
}

// line50 Set up the ignore action
// line53 Set up the default action
// line56 Save the old signal handlers and set new ones sigaction
// line60 Restore the old signal handlers
char	*here_doc_input(char *delimiter)
{
	char				*str;
	char				*buffer;
	struct sigaction	sa_ignore;
	struct sigaction	sa_default;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;

	str = ft_strdup("");
	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, &sa_old_int);
	sigaction(SIGQUIT, &sa_ignore, &sa_old_quit);
	write(STDOUT_FILENO, "> ", 2);
	//heredoc_input_sub(delimiter, str);
	buffer = get_next_line(STDIN_FILENO);
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
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	return (str);
}
