/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:29 by jyap              #+#    #+#             */
/*   Updated: 2024/08/09 18:10:15 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



char	**sort_doublearray(char **array)
{
	int		i;
	int		j;
	int		max;
	char	**output;
	char	*temp;

	output = dup_doublearray(array);
	if (output == NULL)
		return (NULL);
	max = 0;
	while (array[max] != 0)
		max++;
	i = -1;
	while (++i < max)
	{
		j = i;
		while (++j < max)
		{
			if (ft_strcmp(output[i], output[j]) > 0)
			{
				temp = output[i];
				output[i] = output[j];
				output[j] = temp;
			}
		}
	}
	return (output);
}

t_env	*str2env(char *str)
{
	t_env	*env;
	char	*env_key;
	char	*env_value;

	if (ft_strchr(str, '=') == NULL)
	{
		return (new_env(ft_strdup(str), ft_strdup("")));
	}
	env_value = ft_strdup(ft_strchr(str, '=') + 1);
	env_key = ft_substr(str, 0, ft_strlen(str) - ft_strlen(env_value) - 1);
	if (ft_strlen(env_value) == 0)
	{
		free(env_value);
		env_value = ft_strdup("");
	}
	env = new_env(env_key, env_value);
	return (env);
}

bool	is_valid_identifier(char *str)
{
	if (!ft_isalpha(*str) && *str != '_')
		return (false);
	while (*(++str))
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
	}
	return (true);
}


static int	export_assign(t_shell_data *shell_data, char **args)
{
	int		i;
	int		error;
	t_env	*env;

	i = 0;
	error = 0;
	while (args[++i] != 0)
	{
		env = str2env(args[i]);
		if (!is_valid_identifier(env->name))
		{
			error = gen_err(ERR_EXPORTINVALID, args[i], 1);
			continue ;
		}
		append_env(env, &(shell_data->env_data->env_list));
	}
	return (error);
}


int	ft_export(char **args, t_shell_data *shell_data)
{
	char	**arr;
	char	**sorted_arr;
	t_env	*head;

	if (args[1] == NULL)
	{
		arr = env2arr(shell_data->env_data->env_list);
		if (arr == NULL)
			return (0);
		sorted_arr = sort_doublearray(arr);
		if (sorted_arr == NULL)
			return (ft_free_array((void **)arr, 0), 0);
		head = arr2env(arr);
		while (head) {
			if (head->value == NULL)
				printf("declare -x %s\n", head->name);
			else
				printf("declare -x %s=\"%s\"\n", head->name, head->value);
			head = head->next;
		}
		ft_free_array((void **)sorted_arr, 0);
		ft_free_array((void **)arr, 0);
		free_env_list(head);
	}
	else
		export_assign(shell_data, args);
	return (0);
}