#include "minishell.h"


int	bash_cmd(char **env, t_token_info *token_info)
{
	char	**args;
	int		i;

	args = token_info->cmd_start->tokens;
	if ((!ft_strcmp(args[0], "echo")) && args[1] && !ft_strcmp(args[1], "-n"))
	{
		i = 1;
		while (args[++i])
			printf("%s", args[i]);
	}
	if (!ft_strcmp(args[0], "exit"))
	{
		free_TokenList(token_info);
		exit(0);
	}
	if (!ft_strcmp(args[0], "cd"))
		chdir(args[1]);
	if (!ft_strcmp(args[0], "env"))
		print_env(&token_info->global_env, 'e');
	if (!ft_strcmp(args[0], "unset") && args[1])
		unset_env(args[1], &token_info->global_env, token_info);
	if (!ft_strcmp(args[0], "export"))
		export(token_info);
	return (0);
}
