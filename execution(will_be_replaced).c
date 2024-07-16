#include "minishell.h"

/*
THIS FILE WILL BE DELETED SOON. 
*/

//to be replaced by Jyap
char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*sub;
	char	*join;
	char	*pathjoin;

	path = getenv("PATH");
	while (path && *path)
	{
		sub = ft_substr(path, 0, (size_t)(ft_strchr(path, ':') - path));
		join = ft_strjoin(sub, "/");
		pathjoin = ft_strjoin(join, cmd);
		if (access(pathjoin, F_OK) == 0)
			return (f(join, f(sub, (void *)pathjoin)));
		f(pathjoin, f(join, f(sub, NULL)));
		path = ft_strchr(path, ':');
		if (path)
			path ++;
	}
	return ("\0");
}

//to be replaced by Jyap
void	run_cmds(char **env, t_token_info *token_info)
{
	char	*command;
	int		pid;
	char	**args;

	args = token_info->cmd_start->cmds;
	command = get_path(args[0], env);
	pid = fork();
	if (pid != 0)
	{
		wait(0);
		if (command[0] != '\0')
			free(command);
	}
	else if (pid == 0)
	{
		if (access(command, F_OK) == 0)
			execve((const char *)command, (char *const *)args, env);
		exit(0);
	}
}