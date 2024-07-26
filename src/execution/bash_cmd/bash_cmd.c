#include "minishell.h"

bool	is_echo_n(char *str)
{
	int	i;

	i = 0;
	if (str[0] != '-')
		return (false);

	while (str[++i])
	{
		if (str[i] != 'n')
			return (false);
	}
	return (true);
}

int	bash_cmd(char **env, t_token_info *token_info, char **args)
{
	int		i;

	if (!ft_strcmp(args[0], "echo"))
		ft_echo(args);
	else if (!ft_strcmp(args[0], "exit"))
	{
		free_TokenList(token_info);
		ft_exit(0, token_info);
	}
	else if (!ft_strcmp(args[0], "env"))
		print_env(token_info->global_env, 'e');
	else if (!ft_strcmp(args[0], "unset"))
		unset_env(args[1], token_info->global_env, token_info);
	else if (!ft_strcmp(args[0], "export"))
		ft_export(args, token_info);
	return (0);
}
