/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:41 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:41 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_error(char *error_name)
{
	perror(error_name);
	exit(EXIT_FAILURE);
}

int	run_cmd(t_token *chunk, t_shell_data *shell_data,
			int cmd_in_fd, int cmd_out)
{
	if (str_in_arr(chunk->start->word, "exit"))
		ft_exit(chunk->tokens, shell_data);
	else if (str_in_arr(chunk->start->word, "cd"))
	{
		if (chunk->tokens[2])
		{
			g_exit_status = 1;
			ft_putstr_fd("minishell: cd: too many arugments", 2);
			return (0);
		}
		if (chdir(chunk->tokens[1]) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			perror(chunk->tokens[1]);
			g_exit_status = 1;
		}
	}
	else if (str_in_arr(chunk->start->word, "unset"))
		unset_env(chunk->tokens + 1,
			&(shell_data->env_data->env_list), shell_data);
	else if (str_in_arr(chunk->start->word, "export") && chunk->tokens[1])
		ft_export(chunk->tokens, shell_data);
	else
		exec_cmd(chunk->tokens, shell_data, cmd_in_fd, cmd_out);
	return (1);
}

void	child(t_chunk *chunk_list, t_shell_data *shell_data)
{
	signal(SIGTERM, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	set_child_redirections(shell_data);
	run_cmd(chunk_list, shell_data, STDIN_FILENO, STDOUT_FILENO);
	exit(g_exit_status);
}

void	parent(t_chunk *chunk_list, t_shell_data *shell_data)
{
	if (shell_data->executor->cmd_in != STDIN_FILENO)
		close(shell_data->executor->cmd_in);
	if (shell_data->executor->cmd_out != STDOUT_FILENO
		&& shell_data->executor->cmd_out != shell_data->executor->pipefd[1])
		close(shell_data->executor->cmd_out);
	if (chunk_list->next)
	{
		shell_data->executor->cmd_in = shell_data->executor->pipefd[0];
		close(shell_data->executor->pipefd[1]);
	}
	else
	{
		close(shell_data->executor->pipefd[1]);
		close(shell_data->executor->pipefd[0]);
	}
}

/*void	executor(char **env, t_shell_data *shell_data)
{
	t_chunk		*chunk_list;
	pid_t		pid;
	pid_t		last_pid;
	int			status;

	status = 0;
	last_pid = 0;
	chunk_list = get_chunk_start(shell_data->token_chunks,
			shell_data->start_pos);
	if (!chunk_list)
		return ;
	shell_data->executor = executor_init();
	signal(SIGINT, ignore_sigint);
	while (chunk_list)
	{
		set_outf(chunk_list, shell_data->executor);
		set_inf(shell_data->executor, chunk_list, shell_data);
		if (!shell_data->token_chunks->next)
			run_cmd(chunk_list, shell_data,
				shell_data->executor->cmd_in, shell_data->executor->cmd_out);
		else
		{
			pid = fork();
			if (pid == -1)
				exit_error("fork");
			if (pid == 0)
				child(chunk_list, shell_data);
			last_pid = pid;
			parent(chunk_list, shell_data);
		}
		chunk_list = chunk_list->next;
	}
	if (last_pid != 0)
	{
		waitpid(last_pid, &status, 0);
		g_exit_status = WEXITSTATUS(status);
	}
	signal(SIGINT, ctrl_c_function);
	free(shell_data->executor);
}*/

void	executor(char **env, t_shell_data *shell_data)
{
	t_chunk		*chunk_list;
	pid_t		pid;
	int			i;
	int			*pid_array;
	t_chunk		*size;

	i = 0;
	chunk_list = get_chunk_start(shell_data->token_chunks,
			shell_data->start_pos);
	if (!chunk_list)
		return ;
	shell_data->executor = executor_init();
	signal(SIGINT, ignore_sigint);
	size = shell_data->token_chunks;
	shell_data->size = 0;
	while (size)
	{
		shell_data->size++;
		size = size->next;
	}
	pid_array = malloc(sizeof(int) * shell_data->size);
	while (chunk_list)
	{
		set_outf(chunk_list, shell_data->executor);
		set_inf(shell_data->executor, chunk_list, shell_data);
		if (!shell_data->token_chunks->next)
			run_cmd(chunk_list, shell_data,
				shell_data->executor->cmd_in, shell_data->executor->cmd_out);
		else
		{
			pid = fork();
			if (pid == -1)
				exit_error("fork");
			if (pid == 0)
				child(chunk_list, shell_data);
			else
			{
				pid_array[i] = pid;
				parent(chunk_list, shell_data);
			}
			i++;
		}
		chunk_list = chunk_list->next;
	}
	i = 0;
	while (i < shell_data->size)
	{
		waitpid(pid_array[i], &g_exit_status, 0);
		i++;
	}
	free(pid_array);
	signal(SIGINT, ctrl_c_function);
	free(shell_data->executor);
}