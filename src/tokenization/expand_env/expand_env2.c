#include "minishell.h"

bool	is_pure_env(char *str)
{
	char	*ref;

	if (str[0] != '$')
		return (false);
	if (ft_strchr(str, '\'') || ft_strchr(str, '"'))
		return (false);
	ref = str;
	while (1)
	{
		str += 1;
		str += find_env_end(str);
		if (find_env_end(str) == 0)
		{
			if ((size_t)(str - ref) == ft_strlen(ref))
				return (true);
			else
				return (false);
			break ;
		}
		else if (str[1] == '$')
			return (false);
	}
	return (true);
}

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
