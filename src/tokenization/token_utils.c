/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:05:52 by shechong          #+#    #+#             */
/*   Updated: 2024/08/14 18:55:40 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_token	*tok(char *word, char type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->word = word;
	token->type = type;
	token->infile = NULL;
	token->outfile = NULL;
	token->start = NULL;
	token->next = NULL;
	token->tokens = NULL;
	token->outfile_mode = 0;
	token->heredoc_fd = NULL;
	return (token);
}

t_token	*append(t_token *token, t_token **head)
{
	t_token	*nav;

	if (!(token->word))
	{
		free(token);
		return (NULL);
	}
	if (!(*head))
	{
		*head = token;
		token->prev = NULL;
		return (token);
	}
	nav = *head;
	while (nav && nav->next)
		nav = nav->next;
	nav->next = token;
	token->prev = nav;
	return (token);
}
