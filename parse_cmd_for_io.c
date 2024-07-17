#include "minishell.h"

char *here(char *delimiter)
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

int redir_set(char *str, char *filename, t_io *io_list, t_token_info *token_info)
{
	if (!ft_strncmp(str, "<<", 2))
	{
		io_list->heredoc_buffer = here(filename);
		printf("%s\n", expand_env(io_list->heredoc_buffer, token_info));
	}
	else if (!ft_strncmp(str, "<", 1))
		io_list->infile_name = filename;
	else if (!ft_strncmp(str, ">>", 2))
		io_list->outfile_name = filename;
	else if (!ft_strncmp(str, ">", 2))
		io_list->outfile_name = filename;
	return (-1);
}

t_io	*io_list_init(int type)
{
	t_io	*io_list;

	io_list = ft_calloc(1, sizeof(t_io));
	io_list->e_type = type;
	io_list->infile_name = NULL;
	io_list->outfile_name = NULL;
	io_list->heredoc_buffer = NULL;
	return (io_list);
}

void	parse_cmd_for_io(t_token *current_token_chunk, t_token_info *token_info)
{
	int	i;
	t_io	*new;
	t_io	*buffer;
	char	**tokens = current_token_chunk->tokens;

	i = -1;
	while (tokens[++i])
	{
		if (str_in_arr(tokens[i], ">>,>,<<,<"))
		{
			redir_set(tokens[i], tokens[i + 1], current_token_chunk->io_list, token_info);
		}
	}
}

void	parse_cmd_list_for_io(t_token_info *token_info)
{
	t_token	*chunk_list;

	chunk_list = token_info->token_chunks;
	while (chunk_list)
	{
		t_io *io_list = io_list_init(-1);
		chunk_list->io_list = io_list;
		parse_cmd_for_io(chunk_list, token_info);
		chunk_list = chunk_list->next;
	}
}