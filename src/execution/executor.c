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
			g_exit_status = 0;
			return 0;
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



t_chunk *get_chunk_start(t_token *start, int pos)
{
	int i = 0;

	while (i < pos)
	{
		start = start->next;
		i++;
	}
	if (!start)
		return (NULL);
	return (start);
}

void	set_outf(t_token *chunk_list, t_executor *exe, int *pipefd)
{
	int	fd;

	if (chunk_list->outfile)
	{
		if (chunk_list->outfile_mode == 'a')
			fd = open(chunk_list->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else
			fd = open(chunk_list->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		exe->cmd_out = fd;
	}
	else if (chunk_list->next)
		exe->cmd_out = pipefd[1];
	else
		exe->cmd_out = STDOUT_FILENO;
}

void	executor(char **env, t_shell_data *shell_data)
{
    t_chunk		*chunk_list;
    int			pipefd[2];
    int			file_fd_in;
    int			file_fd_out;
    pid_t		pid;
    int			heredoc_fd[2];
    int			empty_pipe[2];
    int			i = 0;

    chunk_list = get_chunk_start(shell_data->token_chunks, shell_data->start_pos);
	if (!chunk_list)
		return;
	
    if (pipe(empty_pipe) == -1)
		exit_error("pipe");
	
    shell_data->executor = executor_init();

	signal(SIGINT, ignore_sigint); 
    while (chunk_list)
    {
        if (pipe(pipefd) == -1)
			exit_error("pipe");

        set_outf(chunk_list, shell_data->executor, pipefd);

        if (chunk_list->infile)
        {
            file_fd_in = open(chunk_list->infile, O_RDONLY);
            if (file_fd_in == -1)
                exit_error("open infile");
            shell_data->executor->cmd_in = file_fd_in;
        }
        else if (chunk_list->heredoc_buffer != NULL)
        {
            if (pipe(heredoc_fd) == -1)
                exit_error("pipe");
            ft_putstr_fd(chunk_list->heredoc_buffer, heredoc_fd[1]);
            close(heredoc_fd[1]);
            shell_data->executor->cmd_in = heredoc_fd[0];
        }
        else if (shell_data->start_pos != 0)
        {
            shell_data->executor->cmd_in = empty_pipe[0];
            close(empty_pipe[1]);
        }
		
		if (!shell_data->token_chunks->next)
			run_cmd(chunk_list, shell_data, shell_data->executor->cmd_in, shell_data->executor->cmd_out);
		else
		{
			pid = fork();
			if (pid == -1)
				exit_error("fork");
			if (pid == 0) // child
			{
				signal(SIGTERM, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				if (shell_data->executor->cmd_in != STDIN_FILENO)
				{
					dup2(shell_data->executor->cmd_in, STDIN_FILENO);
					close(shell_data->executor->cmd_in);
				}
				if (shell_data->executor->cmd_out != STDOUT_FILENO)
				{
					dup2(shell_data->executor->cmd_out, STDOUT_FILENO);
					if (shell_data->executor->cmd_out != pipefd[1])
						close(shell_data->executor->cmd_out);
				}
				close(pipefd[1]);
				close(pipefd[0]);

				run_cmd(chunk_list, shell_data, STDIN_FILENO, STDOUT_FILENO);
				exit(g_exit_status);
			}
			else // parent
			{
				if (shell_data->executor->cmd_in != STDIN_FILENO)
					close(shell_data->executor->cmd_in);
				if (shell_data->executor->cmd_out != STDOUT_FILENO && shell_data->executor->cmd_out != pipefd[1])
					close(shell_data->executor->cmd_out);
				if (chunk_list->next)
				{
					shell_data->executor->cmd_in = pipefd[0];
					close(pipefd[1]);
				}
				else
				{
					close(pipefd[1]);
					close(pipefd[0]);
				}
			}
			i++;
		}
		chunk_list = chunk_list->next;
    }
    while (wait(&g_exit_status) > 0)
        g_exit_status = WEXITSTATUS(g_exit_status);
	signal(SIGINT, ctrl_c_function);
    if (g_exit_status == 13)
		g_exit_status = 0;
	free(shell_data->executor);
}
