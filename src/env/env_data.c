/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 11:47:36 by shechong          #+#    #+#             */
/*   Updated: 2024/08/05 10:04:27 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_data	*new_env_data(char **env)
{
	t_env_data	*env_data;

	env_data = malloc(sizeof(t_env_data));
	env_data->env_list = arr2env(env);
	env_data->environ_arr = env2arr(env_data->env_list);
	return (env_data);
}

t_env	*arr2env(char **env)
{
	int		i;
	t_env	*env_list;
	char	*var_name;
	char	*var_value;

	i = -1;
	env_list = NULL;
	while (env[++i])
	{
		if (ft_strchr(env[i], '=') == NULL)
			append_env(new_env(env[i], NULL), &env_list);
		else
		{
			var_name = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
			var_value = ft_strdup(ft_strchr(env[i], '=') + 1);
			append_env(new_env(var_name, var_value), &env_list);
		}
	}
	return (env_list);
}

int	count_env(t_env	*env)
{
	int		i;
	t_env	*head;

	i = 0;
	if (env == NULL)
		return (0);
	head = env;
	while (head)
	{
		head = head->next;
		i++;
	}
	return (i);
}

char	**env2arr(t_env *env)
{
	int		i;
	char	**env_arr;
	char	*str;

	i = count_env(env);
	env_arr = malloc(sizeof(char *) * (i + 1));
	if (env_arr == NULL)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value == NULL)
			str = ft_strdup(env->name);
		else
		{
			str = ft_strjoin(env->name, "=");
			ft_rstrjoin(&str, env->value);
		}
		env_arr[i] = str;
		env = env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}
