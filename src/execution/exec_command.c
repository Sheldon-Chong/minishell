# include "minishell.h"

char	*get_path(char *cmd, t_env **env)
{
	char	*path;
	char	*sub;
	char	*join;
	char	*pathjoin;

	if (access(cmd, F_OK) == 0)
        return (cmd);
	if (!get_env_var("PATH", env))
		return ("\0");
	path = get_env_var("PATH", env)->value;
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

void reset_ctrl_c_signal() {
	if (signal(SIGINT, SIG_DFL) == SIG_ERR) {
		perror("Failed to reset SIGINT handler");
	}
}


// execute a command with the given arguments, recieving the cmd_in_fd as input and piping its output to cmd_out
void exec_cmd(char **cmd, char **env, t_token_info *token_info, int cmd_in_fd, int cmd_out)
{
	char *command;
	pid_t pid;

	command = get_path(cmd[0], &(token_info->env_data->env_list));
	pid = fork();
	if (pid != 0) // parent
	{
		if (cmd_in_fd != STDIN_FILENO)
			close(cmd_in_fd);
		if (cmd_out != STDOUT_FILENO)
			close(cmd_out);
		waitpid(pid, &g_exit_status, 0); // might have to remove this lines
		if (WIFEXITED(g_exit_status))
			g_exit_status = WIFEXITED(g_exit_status);
		else if (WIFSIGNALED(g_exit_status))
			g_exit_status = WTERMSIG(g_exit_status) + 128;
		else
			g_exit_status = 1;
	}
	else if (pid == 0) //child
	{
		//reset_ctrl_c_signal();
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
		if (str_in_arr(cmd[0], "echo,export,pwd,unset,env"))
		{
			bash_cmd(env, token_info, cmd);
			exit(0);
		}
		if (access(command, F_OK) == 0)
			execve((const char *)command, (char *const *)cmd, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
}
