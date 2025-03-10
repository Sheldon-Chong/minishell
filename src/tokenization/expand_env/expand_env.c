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

static char	*skip_dollar_char(char *env_start)
{
	while (*env_start == '$')
		env_start++;
	return (env_start);
}

static int	expand_env_iter(int *env_end, char *env_start,
				char **buffer, t_shell_data *shell_data)
{
	char	*env_name;
	t_env	*env;

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

static int	init_value(char *string, char **env_start,
	char **i_before, char **buffer)
{
	*env_start = ft_strchr(string, '$') + 1;
	*i_before = string;
	*buffer = ft_strdup("");
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

	if (!ft_strchr(string, '$'))
		return (ft_strdup(string));
	env_end = init_value(string, &env_start, &i_before, &buffer);
	while (*env_start)
	{
		env_start = skip_dollar_char(env_start);
		if (!*env_start)
			break ;
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
