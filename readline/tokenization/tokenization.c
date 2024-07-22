#include "minishell.h"

// expands braces, remove quotes
static char	*process_str(char *str, t_token_info *token_info)
{
	char	*ret;
	t_token	*quote_list_buffer;

	quote_list_buffer = NULL;
	ret = str;
	if (!str[0] || is_token_valid(str, token_info))
		return (NULL);
	t_token	*head = token_info->token_list;
	while(head && head->next)
		head = head->next;
	if (head && !ft_strncmp(head->word, "<<", 2))
		ret = split_into_quotes(str, quote_list_buffer, token_info, false);
	else
		ret = split_into_quotes(str, quote_list_buffer, token_info, true);
	free(str);
	return (ret);
}


// handle cases of shell operators
static int	shell_operator(char *str, int end, t_token **token_list)
{
	char	*string;

	string = str + end;
	if (is_in_strset(string, ">>,<<"))
		return (append_tok(tok(ft_substr(str, end, 2), T_REDIR), token_list), 2);
	else if (is_in_strset(string, ">,<"))
		return (append_tok(tok(ft_substr(str, end, 1), T_REDIR), token_list), 1);
	else if (!ft_strncmp("><", string, 2) || !ft_strncmp("<>", string, 2))
		printf("unexpected character\n");
	else
		return (append_tok(tok(c2str(str[end]), T_PIPE), token_list), 1);
	return (0);
}

static int	form_token(char *str, int end, t_token **token_list)
{
	if (str[end] && is_in_charset(str[end], SHELL_OPERATORS))
		end += shell_operator(str, end, token_list);
	while (is_in_charset(str[end], SPACE_CHAR))
		end ++;
	return (end);
}


// Divides words by spaces, and processes each word before appending them to the token_list
t_token	*tokenize(char *str, t_token_info *token_info)
{
	int		str_end;
	char	quote;

	quote = NOT_WITHIN_QUOTE;
	str_end = 0;
	if (count_outermost_quotes(str) % 2 != 0)
	{
		token_info->has_error = true;
		printf("error: no closing quotation\n");
	}
	while (*str && str[str_end] && !token_info->has_error)
	{
		quote_alternate(str[str_end], &quote);
		if (!quote && (is_in_charset(str[str_end], SPACE_CHAR)
				|| is_in_charset(str[str_end], SHELL_OPERATORS)))
		{
			if (str_end != 0)
				append_tok(tok(process_str(\
				ft_substr(str, 0, str_end), token_info), T_CMD), \
				&(token_info->token_list));
			str += form_token(str, str_end, &(token_info->token_list));
			str_end = 0;
		}
		else
			str_end ++;
	}
	if (!is_in_charset(*str, SPACE_CHAR))
		append_tok(tok(process_str(ft_substr(str, 0, str_end), token_info), T_CMD), 
			&(token_info->token_list));
	return (token_info->token_list);
}
