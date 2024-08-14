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

void	free_env_node(t_env *env)
{
	free(env->name);
	free(env->value);
	free(env);
}

void	set_env_value(t_env *head, char *value)
{
	if (head->value)
		free(head->value);
	head->value = ft_strdup(value);
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
			if (ft_strlen(env->value))
				set_env_value(head, env->value);
			free_env_node(env);
			return ;
		}
		head = head->next;
	}
	head = *envList;
	while (head->next)
		head = head->next;
	head->next = env;
}
