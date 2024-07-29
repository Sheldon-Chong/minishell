#include "minishell.h"

static int	find_env_end(char *env_start)
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

static int	expand_env_iter(int *env_end, char *env_start, char **buffer, t_token_info *token_info)
{
	char	*var_name;
	char	*brace;

	if (*env_start == '?') //return exit status
	{	
		ft_rstrjoin(buffer, ft_itoa(g_exit_status));
		*env_end = 1;
		return 0;
	}
	if (find_env_end(env_start) < 0)
		return (ft_rstrjoin(buffer, "$"), 0);
	*env_end = find_env_end(env_start);
	if (!ft_isalpha(*env_start))
		*env_end = 1;
	else
	{
		var_name = ft_substr(env_start - 1, 0, *env_end + 1);
		if (get_env_var(var_name + 1, &(token_info->env_data->env_list)))
			ft_rstrjoin(buffer, get_env_var(var_name + 1,
					&(token_info->env_data->env_list))->value);
		free(var_name);
	}
	return (0);
}

// returns a string with any occurences of $<text> expanded
char	*expand_env(char *string, t_token_info *token_info)
{
	char	*buffer;
	char	*i_env_begin;
	char	*i_before;
	int		i_env_end;

	i_env_begin = 0;
	i_env_end = 0;
	i_before = string;
	if (!ft_strchr(string, '$'))
		return (string);
	buffer = ft_strdup("");
	i_env_begin = ft_strchr(string, '$') + 1;
	while (*i_env_begin)
	{
		ft_rstrjoin(&buffer, ft_substr(i_before, 0,
				i_env_begin - i_before - 1));
		expand_env_iter(&i_env_end, i_env_begin, &buffer, token_info);
		i_before = i_env_begin + i_env_end;
		if (!ft_strchr(i_env_begin, '$'))
			break ;
		i_env_begin = ft_strchr(i_env_begin, '$') + 1;
	}
	ft_rstrjoin(&buffer, ft_substr(i_before, 0, ft_strlen(i_before)));
	return (buffer);
}
