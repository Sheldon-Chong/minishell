/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:33 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 10:19:46 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_node(t_env *node)
{
	t_env	*temp;

	temp = node;
	node = node->next;
	free (temp->name);
	free (temp->value);
	free (temp);
}

void	unset_env_sub(char **var_names, t_env **envList,
			t_token_info *token_info)
{
	t_env	*head;
	t_env	*prev;
	int		i;

	i = -1;
	while (var_names[++i])
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
				free_env_node(head);
				break ;
			}
			prev = head;
			head = head->next;
		}
	}
}

int	unset_env(char **var_names, t_env **envList, t_token_info *token_info)
{
	unset_env_sub(var_names, envList, token_info);
	ft_free_array((void **)token_info->env_data->environ_arr, 0);
	token_info->env_data->environ_arr = env2arr(*envList);
	return (0);
}
