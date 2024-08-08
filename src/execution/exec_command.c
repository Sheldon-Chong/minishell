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

	if (access(cmd, F_OK) == 0)
		return (cmd);
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

void	reset_signal(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGINT handler");
	if (signal(SIGTERM, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGTERM handler");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGQUIT handler");
}

void	close_fds_and_wait(int cmd_in_fd, int cmd_out, pid_t pid)
{
	if (cmd_in_fd != STDIN_FILENO)
		close(cmd_in_fd);
	if (cmd_out != STDOUT_FILENO)
		close(cmd_out);
	waitpid(pid, &g_exit_status, 0);
	if (WIFEXITED(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
}

// execute a command with the given arguments,
//receiving the cmd_in_fd as input and piping its output to cmd_out
void	exec_cmd(char **cmd, t_token_info *token_info,
			int cmd_in_fd, int cmd_out)
{
	char	*command;
	pid_t	pid;

	g_exit_status = 0;
	pid = fork();
	if (pid == 0)
	{
		reset_signal();
		dup_fd_for_child(cmd_in_fd, cmd_out);
		if (str_in_arr(cmd[0], "echo,export,pwd"))
		{
			bash_cmd(token_info, cmd);
			exit(g_exit_status);
		}
		command = get_path(cmd[0], &(token_info->env_data->env_list));
		if (access(command, F_OK) == 0)
			execve((const char *)command, (char *const *)cmd,
				token_info->env_data->environ_arr);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	close_fds(cmd_in_fd, cmd_out);
	waitpid(pid, &g_exit_status, 0);
	if (WEXITSTATUS(g_exit_status))
		g_exit_status = WEXITSTATUS(g_exit_status);
}
