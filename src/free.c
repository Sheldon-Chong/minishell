/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:03 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:03 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_tokenlist(t_token_info *token_info)
{
	int		i;
	t_token	*head;
	t_token	*temp;

	head = token_info->token_list;
	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->word);
		free(temp);
	}
	head = token_info->token_chunks;
	while (head)
	{
		temp = head;
		head = head->next;
		ft_free_array((void **)temp->tokens, 0);
		free(temp->heredoc_buffer);
		free(temp);
	}
	free(token_info);
	return (0);
}
