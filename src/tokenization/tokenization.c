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

bool	is_pure_env(char *str)
{
	int		i;
	char	*ref;

	i = 0;
	if (str[0] != '$')
		return (false);
	if (ft_strchr(str, '\'') || ft_strchr(str, '"'))
		return (false);
	ref = str;
	while (1)
	{
		str += 1;
		str += find_env_end(str);
		if (find_env_end(str) == 0)
		{
			if (str - ref == ft_strlen(ref))
				return (true);
			else
				return (false);
			break ;
		}
		else if (str[1] == '$')
			return (false);
	}
	return (true);
}

// expands braces, remove quotes
static char	*process_str(char *str, t_shell_data *shell_data)
{
	char	*ret;
	t_token	*quote_list_buffer;
	t_token	*head;

	quote_list_buffer = NULL;
	ret = str;
	if (!str[0] || is_token_valid(str, shell_data))
		return (NULL);
	head = shell_data->token_list;
	while (head && head->next)
		head = head->next;
	if (head && head->type == SH_HEREDOC)
		ret = split_into_quotes(str, quote_list_buffer, shell_data, false);
	else
		ret = split_into_quotes(str, quote_list_buffer, shell_data, true);
	if (ft_strlen(ret) == 0 && is_pure_env(str))
	{
		free(str);
		return (NULL);
	}
	free(str);
	return (ret);
}

// handle cases of shell operators
static int	shell_operator(char *str, int end, t_token **token_list)
{
	char	*string;

	string = str + end;
	if (is_in_strset(string, ">>,<<"))
		return (append(tok(ft_substr(str, end, 2),
					get_shell_opp_type(string)), token_list), 2);
	else if (is_in_strset(string, ">,<"))
		return (append(tok(ft_substr(str, end, 1),
					get_shell_opp_type(string)), token_list), 1);
	else if (!ft_strncmp("><", string, 2) || !ft_strncmp("<>", string, 2))
		printf("unexpected character\n");
	else
		return (append(tok(c2str(str[end]),
					get_shell_opp_type(string)), token_list), 1);
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
t_token	*tokenize(char *str, t_shell_data *shell_data)
{
	int		str_end;
	char	quote;

	quote = NOT_WITHIN_QUOTE;
	str_end = 0;
	while (*str && str[str_end] && !shell_data->has_error)
	{
		quote_alternate(str[str_end], &quote);
		if (!quote && (is_in_charset(str[str_end], SPACE_CHAR)
				|| is_in_charset(str[str_end], SHELL_OPERATORS)))
		{
			if (str_end != 0)
				append(tok(process_str(\
				ft_substr(str, 0, str_end), shell_data), 'c'), \
				&(shell_data->token_list));
			str += form_token(str, str_end, &(shell_data->token_list));
			str_end = 0;
		}
		else
			str_end ++;
	}
	if (!is_in_charset(*str, SPACE_CHAR))
		append(tok(process_str(ft_substr(str, 0, str_end), \
			shell_data), 'c'), &(shell_data->token_list));
	return (shell_data->token_list);
}
