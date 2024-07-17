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

char	**tokens2arr(t_token *token_chunk, t_token *str_end, t_token_info *token_info)
{
	t_token	*token;
	char	**cmds;
	int		i;

	cmds = malloc(1 * sizeof(char *));
	cmds[0] = NULL;
	token = token_chunk->start;
	i = 0;
	while (token != str_end)
	{
		if(str_in_arr(token->word, ">>,>,<<,<"))
		{
			if(!ft_strncmp(token->word, "<<", 2))
			{
				if (token_chunk->infile != NULL)
				{
					free(token_chunk->infile);
					token_chunk->infile = NULL;
				}
				token_chunk->heredoc_buffer = expand_env(here_doc_input(token->next->word), token_info);
			}
			token = token->next->next;
			continue;
		}
		cmds = append_to_array(&cmds,token->word);
		token = token->next;
	}
	i = -1;
	while(cmds[++i])
		printf("%s\n", cmds[i]);
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
