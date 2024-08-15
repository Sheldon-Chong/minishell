/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:06 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:06 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ctrl_c_heredoc(int signum)
{
	(void)signum;
	printf("\n");
	close(STDIN_FILENO);
	g_exit_status = ERRNO_CTRL_C;
}

void	heredoc(char *delimiter, int fd[2], t_shell_data *shell_data, int empty[2])
{
	char	*tmp;
	char	*tmp2;

	close(fd[0]);
	close(fd[1]);
	close(empty[0]);
	close(empty[1]);
	signal(SIGINT, ctrl_c_heredoc);
	tmp2 = NULL;
	while (1)
	{
		tmp = readline("> ");
		if (tmp && ft_strcmp(tmp, delimiter) == 0)
			break ;
		else if (g_exit_status == 130)
		{
			exit(130);
		}
		else if (!tmp)
		{
			printf("minishell: warning: heredoc delimited by eof\n");
			exit(0);
		}
		else
		{
			tmp2 = expand_env(tmp, shell_data);
			ft_putstr_fd(tmp2, fd[1]);
			ft_putstr_fd("\n", fd[1]);
			free(tmp);
			free(tmp2);
		}
	}
	exit(0);
}

void	exec_heredoc(t_chunk *chunk, char *delimiter, t_shell_data *shell_data)
{
	int		*fd;
	int		*empty;
	pid_t	pid;
	int		status;

	fd = ft_calloc(2, sizeof(int));
	empty = ft_calloc(2, sizeof(int));
	if (!delimiter || !*delimiter)
	{
		shell_data->has_error = true;
		return ;
	}
	if (pipe(fd) == -1)
	{
		free(fd);
		return ;
	}
	if (pipe(empty) == -1)
	{
		close(fd[0]);
		close(fd[1]);
		free(fd);
		return ;
	}
	if (chunk->heredoc_fd != NULL)
		close(chunk->heredoc_fd[0]);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		heredoc(delimiter, fd, shell_data, empty);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	if (g_exit_status == 130)
	{
		close(fd[0]);
		close(fd[1]);
		close(empty[1]);
		chunk->heredoc_fd = empty;
		free(fd);
		signal(SIGINT, ctrl_c_function);
		shell_data->has_error = true;
		return ;
	}
	close(fd[1]);
	close(empty[1]);
	close(empty[0]);
	chunk->heredoc_fd = fd;
	free(empty);
	signal(SIGINT, ctrl_c_function);
}
