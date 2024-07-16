#include "minishell.h"

int	newline(int var)
{
	rl_on_new_line();
	return (1);
}

int	is_token_valid(char *str, t_token_info *token_info)
{
	char	quote;
	int		i;

	i = -1;
	quote = 0;
	while (str[++i])
	{
		if (is_in_charset(str[i], "'\""))
			quote = toggle_quote_state(quote, str[i]);
		if (!quote && is_in_charset(str[i], INVALID_CHARS))
		{
			token_info->has_error = true;
			return (printf("bash: syntax error near unexpected token `%c'\n",
					str[i]));
		}
	}
	if (quote)
	{
		return (newline(printf("bash: syntax error near unexpected token `%c'\n",
				str[i])), 1);token_info->has_error = true;
	}
	return (0);
}

char	**tokens2arr(t_token *token_chunk, t_token *str_end)
{
	t_token	*token_chunk_nav;
	char	**cmds;
	int		i;

	token_chunk_nav = token_chunk->start;
	i = 0;
	while (token_chunk_nav != str_end)
	{
		i ++;
		token_chunk_nav = token_chunk_nav->next;
	}
	cmds = malloc(sizeof(char *) * (i + 1));
	cmds[i] = NULL;
	i = 0;
	token_chunk_nav = token_chunk->start;
	while (token_chunk_nav != str_end)
	{
		cmds[i++] = token_chunk_nav->word;
		token_chunk_nav = token_chunk_nav->next;
	}
	return (cmds);
}

int get_length_of_list(t_token *head)
{
	int length = 0;
	while(head)
	{
		length += 1;
		head=head->next;
	}
	return (length);
}
