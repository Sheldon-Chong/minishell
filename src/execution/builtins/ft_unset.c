/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:33 by jyap              #+#    #+#             */
/*   Updated: 2024/08/14 16:39:36 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env_var(char *var_name, t_env **envList)
{
	t_env	*head;
	t_env	*prev;

	head = *envList;
	prev = NULL;
	while (head)
	{
		if (!ft_strcmp(var_name, head->name))
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

void	unset_env_sub(char **var_names, t_env **envList)
{
	int	i;

	i = -1;
	while (var_names[++i])
	{
		if (!is_valid_identifier(var_names[i]))
		{
			gen_err("unset: `,$SUBJECT,': not a valid identifier",
				var_names[i], 1);
			continue ;
		}
		remove_env_var(var_names[i], envList);
	}
}

int	unset_env(char **var_names, t_env **envList, t_shell_data *shell_data)
{
	unset_env_sub(var_names, envList);
	ft_free_array((void **)shell_data->env_data->environ_arr, 0);
	shell_data->env_data->environ_arr = env2arr(*envList);
	return (0);
}
