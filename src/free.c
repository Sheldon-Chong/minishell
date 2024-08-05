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

int	free_TokenList(t_token_info *token_info)
{
	int		i;
	t_token	*head;
	t_token	*temp;

	head = token_info->token_list;
	while (head)
	{
		temp = head->next;
		free(head->word);
		free(head);
		head = temp;
	}
	head = token_info->token_chunks;
	while (head)
	{
		temp = head->next;
		ft_free_array((void **)head->tokens, 0);
		free(head);
		head = temp;
	}
	free(token_info);
	return (0);
}
