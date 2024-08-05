/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:33 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:34 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset_env(char **var_names, t_env **envList, t_token_info *token_info)
{
	t_env	*head;
	t_env	*prev;
	t_env	*temp;
	int		i;

	i = 0;
	while (var_names[i])
	{
		head = *envList;
		prev = NULL;
		while (head)
		{
			if (!ft_strcmp(var_names[i], head->name))
			{
				if (prev)
					prev->next = head->next;
				else
					*envList = head->next;
				temp = head;
				head = head->next;
				free(temp->name);
				free(temp->value);
				free(temp);
				break ;
			}
			prev = head;
			head = head->next;
		}
		i++;
	}
	ft_free_array((void **)token_info->env_data->environ_arr, 0);
	token_info->env_data->environ_arr = env2arr(*envList);
	return (0);
}
