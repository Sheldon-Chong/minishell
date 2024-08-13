/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:16:24 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:16:24 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env(char *name, char *value)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->next = NULL;
	env->name = name;
	env->value = value;
	return (env);
}

void	append_env(t_env *env, t_env **envList)
{
	t_env	*head;

	if (!(*envList))
	{
		*envList = env;
		return ;
	}
	head = *envList;
	while (head)
	{
		if (!ft_strcmp(head->name, env->name))
		{
			if (head->value)
				free(head->value);
			head->value = ft_strdup(env->value);
			free_env_node(env);
		}
		if (head->next)
			head = head->next;
		else
			break ;
	}
	head->next = env;
}
