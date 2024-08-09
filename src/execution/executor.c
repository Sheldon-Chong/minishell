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
	perror("pipe");
	exit(EXIT_FAILURE);
}

int	run_cmd(t_token *chunk, t_token_info *token_info,
			int cmd_in_fd, int cmd_out)
{
	if (str_in_arr(chunk->start->word, "exit"))
		ft_exit(chunk->tokens, token_info);
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
			&(token_info->env_data->env_list), token_info);
	else if (str_in_arr(chunk->start->word, "export") && chunk->tokens[1])
		ft_export(chunk->tokens, token_info);
	else
		exec_cmd(chunk->tokens, token_info, cmd_in_fd, cmd_out);
	return 1;
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

void	executor(char **env, t_token_info *token_info)
{
    t_token		*chunk_list;
    int			pipefd[2];
    int			file_fd_in;
    int			file_fd_out;
    pid_t		pid;
    int			heredoc_fd[2];
    int			empty_pipe[2];
    int			i = 0;

    chunk_list = token_info->token_chunks;

    // Move to the start position
    while (i < token_info->start_pos)
    {
        chunk_list = chunk_list->next;
        i++;
    }
    if (!chunk_list)
        return ;

    // Create an empty pipe
    if (pipe(empty_pipe) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	
    token_info->executor = executor_init();

    while (chunk_list)
    {
		
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        set_outf(chunk_list, token_info->executor, pipefd);

        if (chunk_list->infile)
        {
            file_fd_in = open(chunk_list->infile, O_RDONLY);
            if (file_fd_in == -1)
                exit_error("open infile");
            token_info->executor->cmd_in = file_fd_in;
        }
        else if (chunk_list->heredoc_buffer != NULL)
        {
            if (pipe(heredoc_fd) == -1)
                exit_error("pipe");
            ft_putstr_fd(chunk_list->heredoc_buffer, heredoc_fd[1]);
            close(heredoc_fd[1]);
            token_info->executor->cmd_in = heredoc_fd[0];
        }
        else if (i == token_info->start_pos && token_info->start_pos != 0)
        {
            // Redirect stdin of the first chunk to the empty pipe
            token_info->executor->cmd_in = empty_pipe[0];
            close(empty_pipe[1]);
        }
		
		if (!token_info->token_chunks->next)
		{
			run_cmd(chunk_list, token_info, token_info->executor->cmd_in, token_info->executor->cmd_out);
		}
		else
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if (pid == 0) // child
			{
				signal(SIGTERM, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				if (token_info->executor->cmd_in != STDIN_FILENO)
				{
					dup2(token_info->executor->cmd_in, STDIN_FILENO);
					close(token_info->executor->cmd_in);
				}
				if (token_info->executor->cmd_out != STDOUT_FILENO)
				{
					dup2(token_info->executor->cmd_out, STDOUT_FILENO);
					if (token_info->executor->cmd_out != pipefd[1])
						close(token_info->executor->cmd_out);
				}
				close(pipefd[1]);
				close(pipefd[0]);

				run_cmd(chunk_list, token_info, STDIN_FILENO, STDOUT_FILENO);
				exit(g_exit_status);
			}
			else // parent
			{
				if (token_info->executor->cmd_in != STDIN_FILENO)
					close(token_info->executor->cmd_in);
				if (token_info->executor->cmd_out != STDOUT_FILENO && token_info->executor->cmd_out != pipefd[1])
					close(token_info->executor->cmd_out);
				if (chunk_list->next)
				{
					token_info->executor->cmd_in = pipefd[0];
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
    free(token_info->executor);
}