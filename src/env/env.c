/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:16:57 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:16:57 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env_var(char *var_name, t_env **env)
{
	t_env	*head;

	head = *env;
	while (head)
	{
		if (ft_strcmp(head->name, var_name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}



void	free_env_data(t_env_data *env_data)
{
	t_env	*head;
	t_env	*tmp;

	head = env_data->env_list;
	while (head)
	{
		tmp = head;
		free(head->name);
		free(head->value);
		head = head->next;
		free(tmp);
	}
	ft_free_array((void **)((env_data)->environ_arr), 0);
	free(env_data);
}



void free_env_list(t_env *env)
{
	t_env *temp;
	while (env)
	{
		free(env->name);
		if (env->value)
			free(env->value);
		temp = env->next;
		free(env);
		env = temp;
	}
}