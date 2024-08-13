#include "minishell.h"

t_chunk	*new_chunk(t_token *start)
{
	t_chunk	*new_chunk;

	new_chunk = tok("", 'g');
	new_chunk->start = start;
	return (new_chunk);
}




int	handle_redir(
				t_token *head,
				t_token *token_chunk,
				t_shell_data *shell_data,
				int num)
{
	int		file;
	int		e;

	e = 0;
	if (head->type == SH_APPEND || head->type == SH_WRITE)
	{
		file = open(head->next->word, O_CREAT | O_RDWR, 0644);
		if (file == -1)
		{
			e = gen_err(ERR_NOPERM, head->next->word, 1);
			shell_data->start_pos = num + e;
			return (-1);
		}
		close(file);
		if (head->type == SH_APPEND)
			token_chunk->outfile_mode = 'a';
		token_chunk->outfile = head->next->word;
	}
	if (head->type == SH_READ)
	{
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
	return (0);
}
