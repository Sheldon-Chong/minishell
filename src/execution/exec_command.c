/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:36 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:36 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*get_path(char *cmd, t_env **env)
{
	char	*path;
	char	*sub;
	char	*join;
	char	*pathjoin;

	if (!get_env_var("PATH", env))
		return (NULL);
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
	return (NULL);
}

int	check_path_type(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (P_NOT_EXIST);
	if (S_ISDIR(path_stat.st_mode))
		return (P_DIR);
	else if (S_ISREG(path_stat.st_mode))
		return (P_FILE);
	else
		return (3);
}

void	gen_err_and_exit(char *str, char *subject, int num, int exit_num)
{
	gen_err(str, subject, num);
	exit (exit_num);
}

void	check_path_before_execve(char **command, char *cmd)
{
	int	path_type;

	if (*command == NULL)
	{
		path_type = check_path_type(cmd);
		if (ft_strchr(cmd, '/'))
		{
			if (path_type == P_NOT_EXIST)
				gen_err_and_exit(ERR_NOFILDIR, cmd, 1, 127);
			else if (path_type == P_DIR)
				gen_err_and_exit(ERR_ISDIR, cmd, 1, 126);
			else if (path_type == P_FILE && access(cmd, X_OK) != 0)
				gen_err_and_exit(ERR_NOPERM, cmd, 1, 126);
			else
				*command = cmd;
		}
		else
			gen_err_and_exit(ERR_CMDNOTFOUND, cmd,
				ERRNO_COMMAND_NOT_FOUND, 127);
	}
}

// execute a command with the given arguments,
// receiving the cmd_in_fd as input and piping its output to cmd_out
void	exec_cmd(char **cmd, t_shell_data *shell_data,
			int cmd_in_fd, int cmd_out)
{
	char	*command;
	pid_t	pid;
	int		status;

	g_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		reset_signal();
		dup_fd_for_child(cmd_in_fd, cmd_out);
		if (str_in_arr(cmd[0], "echo,export,pwd,unset,env"))
		{
			builtins(shell_data, cmd);
			exit(g_exit_status);
		}
		command = get_path(cmd[0], &(shell_data->env_data->env_list));
		check_path_before_execve(&command, cmd[0]);
		execve((const char *)command, (char *const *)cmd,
			shell_data->env_data->environ_arr);
		gen_err_and_exit(ERR_CMDNOTFOUND, cmd[0], ERRNO_COMMAND_NOT_FOUND, 127);
	}
	close_fds(cmd_in_fd, cmd_out);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}
