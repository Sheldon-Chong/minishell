/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:48 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:48 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Join the quote linkedlist back into a single string
static char	*quote_list2str(t_subquote *quote_list)
{
	t_subquote	*head;
	char		*buffer;
	char		*buffer2;
	t_subquote	*temp;

	head = quote_list;
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
static void	process_quote_list(t_subquote *head,
	t_shell_data *shell_data, bool is_expand_env)
{
	char	*temp;

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
		if (is_expand_env)
		{
			temp = head->word;
			head->word = expand_env(head->word, shell_data);
			free(temp);
		}
		head = head->next;
	}
}

static void	split_into_quotes_sub(char *str, t_token **tokens)
{
	int		i;
	char	quote;
	int		pos;
	char	*string;
	int		len;

	pos = 0;
	quote = '\0';
	i = -1;
	while (str[++i])
	{
		if (!is_char_transition_quote(quote, str[i]))
			continue ;
		quote = toggle_quote_state(quote, str[i]);
		len = i - pos + !(quote);
		string = ft_substr(str, pos, len);
		append(tok(ft_strdup(string), 0), tokens);
		free(string);
		pos += len;
	}
	len = i - pos;
	string = ft_substr(str, pos, len);
	append(tok(ft_strdup(string), 0), tokens);
	free(string);
}

// Forms a linkedlist with quotes
char	*split_into_quotes(char *str, t_token *tokens,
	t_shell_data *shell_data, bool expand_env)
{
	split_into_quotes_sub(str, &tokens);
	process_quote_list(tokens, shell_data, expand_env);
	return (quote_list2str(tokens));
}
