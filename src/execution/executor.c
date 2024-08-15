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

int	run_cmd(t_token *chunk, t_shell_data *shell_data,
			int cmd_in_fd, int cmd_out)
{
	if (str_in_arr(chunk->start->word, "exit"))
		ft_exit(chunk->tokens, shell_data);
	else if (str_in_arr(chunk->start->word, "cd"))
		ft_cd(chunk);
	else if (str_in_arr(chunk->start->word, "unset"))
		unset_env(chunk->tokens + 1,
			&(shell_data->env_data->env_list), shell_data);
	else if (str_in_arr(chunk->start->word, "export") && chunk->tokens[1])
		ft_export(chunk->tokens, shell_data);
	else
	{
		signal(SIGINT, SIG_IGN);
		exec_cmd(chunk->tokens, shell_data, cmd_in_fd, cmd_out);
	}
	return (1);
}

/*
void	executor(char **env, t_shell_data *shell_data)
{
	t_chunk		*chunk_list;
	pid_t		pid;
	int			status;
	pid_t		pid_list[1024];

	status = 0;
	pid_count = 0;
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
			pid_list[pid_count++] = pid;
			parent(chunk_list, shell_data);
		}
		chunk_list = chunk_list->next;
	}
	for (int i = 0; i < pid_count; i++)
	{
		waitpid(pid_list[i], &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
	signal(SIGINT, ctrl_c_function);
	free(shell_data->executor);
}
*/

void	init_value_for_executor(t_shell_data *shell_data, int *lastpid,
	int *size, t_chunk **chunk_list)
{
	*lastpid = 0;
	*size = get_length_of_list(shell_data->token_chunks);
	*chunk_list = get_chunk_start(shell_data->token_chunks,
			shell_data->start_pos);
	shell_data->executor = executor_init();
	signal(SIGINT, ignore_sigint);
}

void	run_cmd_fork(int *lastpid, t_chunk *chunk_list,
	t_shell_data *shell_data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit_error("fork");
	if (pid == 0)
		child(chunk_list, shell_data);
	*lastpid = pid;
	parent(chunk_list, shell_data);
}

void	wait_for_child_and_cleanup(int lastpid, int size,
	t_shell_data *shell_data)
{
	int	status;

	if (lastpid != 0)
	{
		waitpid(lastpid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
	while (--size > 0)
		waitpid(-1, NULL, 0);
	signal(SIGINT, ctrl_c_function);
	free(shell_data->executor);
}

void	executor(t_shell_data *shell_data)
{
	t_chunk		*chunk_list;
	pid_t		lastpid;
	int			size;

	init_value_for_executor(shell_data, &lastpid, &size, &chunk_list);
	if (!chunk_list)
		return ;
	while (chunk_list)
	{
		set_outf(chunk_list, shell_data->executor);
		set_inf(shell_data->executor, chunk_list, shell_data);
		if (!shell_data->token_chunks->next)
			run_cmd(chunk_list, shell_data,
				shell_data->executor->cmd_in, shell_data->executor->cmd_out);
		else
			run_cmd_fork(&lastpid, chunk_list, shell_data);
		chunk_list = chunk_list->next;
	}
	wait_for_child_and_cleanup(lastpid, size, shell_data);
}
