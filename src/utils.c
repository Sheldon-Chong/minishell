/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:14 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:14 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			return (printf("minishell: syntax error near unexpected token `%c'\n",
					str[i]));
		}
	}
	if (quote)
	{
		token_info->has_error = true;
		printf("minishell: syntax error near unexpected token `%c'\n", str[i]);
		rl_on_new_line();
		return (1);
	}
	return (0);
}

void	clear_heredoc_buffer(t_token *chunk)
{
	if (chunk->infile != NULL)
	{
		free(chunk->infile);
		chunk->infile = NULL;
	}
	if (chunk->heredoc_buffer != NULL)
	{
		free(chunk->heredoc_buffer);
		chunk->heredoc_buffer = NULL;
	}
}

char	**tokens2arr(t_token *chunk, t_token *str_end, t_token_info *token_info)
{
	t_token	*token;
	char	**cmds;

	cmds = calloc(1, 2 * sizeof(char *));
	token = chunk->start;
	
	while (token != str_end)
	{
		if (str_in_arr(token->word, ">>,>,<<,<"))
		{
			if (!ft_strncmp(token->word, "<<", 2))
			{
				clear_heredoc_buffer(chunk);
				chunk->heredoc_buffer = here_doc_input(token->next->word);
			}
			token = token->next->next;
			continue ;
		}
		cmds = append_to_array(&cmds, token->word);
		token = token->next;
	}
	return (cmds);
}

int	get_length_of_list(t_token *head)
{
	int	length;

	length = 0;
	while (head)
	{
		length++;
		head = head->next;
	}
	return (length);
}
