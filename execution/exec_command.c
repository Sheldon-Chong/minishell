# include "minishell.h"

static char	*get_path(char *cmd, char **env)
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

// execute a command with the given arguments, recieving the cmd_in_fd as input and piping its output to cmd_out
void exec_cmd(char **cmd, char **env, t_token_info *token_info, int cmd_in_fd, int cmd_out)
{
	char *command;
	pid_t pid;

	command = get_path(cmd[0], env);
	pid = fork();
	if (pid != 0) // parent
	{
		if (cmd_in_fd != STDIN_FILENO)
			close(cmd_in_fd);
		if (cmd_out != STDOUT_FILENO)
			close(cmd_out);
		waitpid(pid, NULL, 0);
		if (command[0] != '\0')
			free(command);
	}
	else if (pid == 0) //child
	{
		if (cmd_in_fd != STDIN_FILENO)
		{
			dup2(cmd_in_fd, STDIN_FILENO);
			close(cmd_in_fd);
		}
		if (cmd_out != STDOUT_FILENO)
		{
			dup2(cmd_out, STDOUT_FILENO);
			close(cmd_out);
		}
		if (access(command, F_OK) == 0)
			execve((const char *)command, (char *const *)cmd, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}
