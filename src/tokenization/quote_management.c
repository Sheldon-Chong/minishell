#include "minishell.h"

// Join the quote linkedlist back into a single string
static char	*quote_list2str(t_token *token_list)
{
	t_token	*head;
	char	*buffer;
	char	*buffer2;
	t_token	*temp;

	head = token_list;
	buffer = ft_strdup("");
	while (head)
	{
		temp = head->next;
		buffer2 = head->word;
		ft_rstrjoin(&buffer, buffer2);
		free(head);
		free(buffer2);
		head = temp;
	}
	return (buffer);
}

// Remove the outermost quotes, expand any occurences of '$'
static void	process_quote_list(t_token *head, t_token_info *token_info)
{
	while (head)
	{
		if (head->word[0] == '\'')
		{
			head->word = ft_rsubstr(&(head->word), 1,
					ft_strlen(head->word) - 2);
			head = head->next;
			continue ;
		}
		else if (head->word[0] == '"')
			head->word = ft_rsubstr(&(head->word), 1,
					ft_strlen(head->word) - 2);
		head->word = expand_env(head->word, token_info);
		head = head->next;
	}
}

// Forms a linkedlist with quotes
char	*split_into_quotes(char *str, t_token *tokens,
	t_token_info *token_info, bool expand_env)
{
	int		i;
	char	quote;
	int		start;
	int		end;
	char	*string;

	i = -1;
	quote = '\0';
	start = 0;
	end = 0;
	while (str[++i])
	{
		if ((is_in_charset(str[i], "'\""))
			&& (!quote || str[i] == quote))
		{
			quote = toggle_quote_state(quote, str[i]);
			if (quote)
			{
				string = ft_substr(str, end, i - end);
				if (string[0])
					append_tok(tok(ft_strdup(string), 0), &(tokens));
				start = i;
			}
			else
			{
				string = ft_substr(str, start, i - start + 1);
				if (string[0])
					append_tok(tok(ft_strdup(string), 0), &(tokens));
				end = i + 1;
			}
			free(string);
		}
	}
	string = ft_substr(str, end, i);
	if (string[0])
		append_tok(tok(ft_strdup(string), 'o'), &(tokens));
	free(string);
	if (expand_env)
		process_quote_list(tokens, token_info);
	return (quote_list2str(tokens));
}
