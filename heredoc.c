#include "minishell.h"

char *here_doc_input(char *delimiter)
{
	write(STDOUT_FILENO, "> ", 2);
	char *buffer = get_next_line(STDIN_FILENO);
	char *str = ft_strdup("");

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