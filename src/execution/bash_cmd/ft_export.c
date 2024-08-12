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

char	**dup_doublearray(char **src)
{
    char	**output;
    int		i;

    i = 0;
    while (src[i] != NULL)
        i++;
    output = ft_calloc(i + 1, sizeof(char *));
    if (output == NULL)
        return (NULL);
    output[i] = NULL;
    while (--i >= 0)
    {
        output[i] = ft_strdup(src[i]);
        if (output[i] == NULL)
        {
            ft_free_array((void **)output, 0);
            return (NULL);
        }
    }
    return (output);
}

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
        return (new_env(ft_strdup(str), NULL));
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

    head = shell_data->env_data->env_list;
    if (args[1] == NULL)
    {
		free(shell_data->env_data->environ_arr);
		t_env *head = shell_data->env_data->env_list;
        arr = env2arr(shell_data->env_data->env_list);
        if (arr == NULL)
            return (0);
        sorted_arr = sort_doublearray(arr);
        if (sorted_arr == NULL)
        {
            ft_free_array((void **)arr, 0);
            return (0);
        }
        while (head)
        {
            free(head->name);
            free(head->value);
			t_env *temp = head->next;
            free(head);
			head = temp;
        }
        shell_data->env_data->env_list = arr2env(sorted_arr);
        print_env(&(shell_data->env_data->env_list), 'x');
        ft_free_array((void **)sorted_arr, 0); // Free the sorted array
        ft_free_array((void **)arr, 0); // Free the original array
    }
    else
        export_assign(shell_data, args);
    return (0);
}