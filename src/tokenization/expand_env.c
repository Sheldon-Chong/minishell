/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:46 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:46 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_env_end(char *env_start)
{
	int	i;

	if (env_start[0] == '$')
		return (-1);
	if (!ft_isalpha(env_start[0]) && env_start[0] != '_')
		return (0);
	i = 1;
	while (env_start[i])
	{
		if (!ft_isalnum(env_start[i]) && env_start[i] != '_')
			break ;
		i ++;
	}
	return (i);
}

static int	expand_env_iter(int *env_end, char *env_start,
				char **buffer, t_shell_data *shell_data)
{
	char	*env_name;
	t_env	*env;
	char	*brace;

	if (*env_start == '?')
	{
		ft_rstrjoin(buffer, ft_itoa(g_exit_status));
		*env_end = 1;
		return (0);
	}
	if (find_env_end(env_start) < 1)
		return (ft_rstrjoin(buffer, "$"), 0);
	*env_end = find_env_end(env_start);
	if (!ft_isalpha(*env_start))
		*env_end = 1;
	else
	{
		env_name = ft_substr(env_start - 1, 0, *env_end + 1);
		env = get_env_var(env_name + 1, &(shell_data->env_data->env_list));
		if (env)
			ft_rstrjoin(buffer, env->value);
		free(env_name);
	}
	return (0);
}

// returns a string with any occurences of $<text> expanded
char	*expand_env(char *string, t_shell_data *shell_data)
{
	char	*buffer;
	char	*env_start;
	char	*i_before;
	int		env_end;
	char	*str;

	env_start = ft_strchr(string, '$') + 1;
	env_end = 0;
	i_before = string;
	if (!ft_strchr(string, '$'))
		return (ft_strdup(string));
	buffer = ft_strdup("");
	while (*env_start)
	{
		while (*env_start == '$')
			env_start ++; 
		if (!*env_start)
			break;
		str = ft_substr(i_before, 0, env_start - i_before - 1);
		buffer = ft_fstrjoinf(&buffer, &str);
		expand_env_iter(&env_end, env_start, &buffer, shell_data);
		i_before = env_start + env_end;
		if (!ft_strchr(env_start, '$'))
			break ;
		env_start = ft_strchr(env_start, '$') + 1;
	}
	str = ft_substr(i_before, 0, ft_strlen(i_before));
	buffer = ft_fstrjoinf(&buffer, &str);
	return (buffer);
}
