/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:50 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:50 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// expands braces, remove quotes
static char	*process_str(char *str, t_token_info *token_info)
{
	char	*ret;
	t_token	*quote_list_buffer;
	t_token	*head;

	quote_list_buffer = NULL;
	ret = str;
	if (!str[0] || is_token_valid(str, token_info))
		return (NULL);
	head = token_info->token_list;
	while (head && head->next)
		head = head->next;
	if (head && head->type == SH_HEREDOC)
		ret = split_into_quotes(str, quote_list_buffer, token_info, false);
	else
		ret = split_into_quotes(str, quote_list_buffer, token_info, true);
	free(str);
	return (ret);
}

static int get_shell_opp_type(char *str)
{
	if (!strncmp(str, ">>", 2))
		return SH_APPEND;
	else if (!strncmp(str, ">", 1))
		return SH_WRITE;
	else if (!strncmp(str, "<<", 2))
		return SH_HEREDOC;
	else if (!strncmp(str, "<", 1))
		return SH_READ;
	else if (!strncmp(str, "|", 1))
		return SH_PIPE;
	return (-1);
}

// handle cases of shell operators
static int	shell_operator(char *str, int end, t_token **token_list)
{
	char	*string;

	string = str + end;
	if (is_in_strset(string, ">>,<<"))
		return (append_tok(tok(ft_substr(str, end, 2), get_shell_opp_type(string)), token_list), 2);
	else if (is_in_strset(string, ">,<"))
		return (append_tok(tok(ft_substr(str, end, 1), get_shell_opp_type(string)), token_list), 1);
	else if (!ft_strncmp("><", string, 2) || !ft_strncmp("<>", string, 2))
		printf("unexpected character\n");
	else
		return (append_tok(tok(c2str(str[end]), get_shell_opp_type(string)), token_list), 1);
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

// Divides words by spaces. Processes each word before appending to token_list
t_token	*tokenize(char *str, t_token_info *token_info)
{
	int		str_end;
	char	quote;

	quote = NOT_WITHIN_QUOTE;
	str_end = 0;
	while (*str && str[str_end] && !token_info->has_error)
	{
		quote_alternate(str[str_end], &quote);
		if (!quote && (is_in_charset(str[str_end], SPACE_CHAR)
				|| is_in_charset(str[str_end], SHELL_OPERATORS)))
		{
			if (str_end != 0)
				append_tok(tok(process_str(\
				ft_substr(str, 0, str_end), token_info), 'c'), \
				&(token_info->token_list));
			str += form_token(str, str_end, &(token_info->token_list));
			str_end = 0;
		}
		else
			str_end ++;
	}
	if (!is_in_charset(*str, SPACE_CHAR))
		append_tok(tok(process_str(ft_substr(str, 0, str_end), \
			token_info), 'c'), &(token_info->token_list));
	return (token_info->token_list);
}
