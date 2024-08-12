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

char	*heredoc_input_sub(char *delimiter)
{
	char	*buffer;
	char	*str;

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
	return (str);
}

void signal_init(void)
{
	struct sigaction	sa_ignore;
	struct sigaction	sa_default;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	sigaction(SIGINT, &sa_ignore, NULL);
	sigaction(SIGQUIT, &sa_ignore, NULL);
}

char	*here_doc_input(char *delimiter, t_shell_data *shell_data)
{
	char				*str;
	struct sigaction	sa_old_int;
	struct sigaction	sa_old_quit;

	signal_init();
	write(STDOUT_FILENO, "> ", 2);
	str = heredoc_input_sub(delimiter);
	sigaction(SIGINT, &sa_old_int, NULL);
	sigaction(SIGQUIT, &sa_old_quit, NULL);
	str = expand_env(str, shell_data);
	return (str);
}
