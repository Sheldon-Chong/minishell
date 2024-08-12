/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:44 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:44 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// line25 Check if the path exists
// line27 Check if the path is a directory
// line32 Path exists but is not a directory


int	is_dir(char *path)
{
	struct stat	statbuf;
	int			fd;


	if (!ft_strchr(path, '/'))
		return (PATH_IS_FILE);
	if (stat(path, &statbuf) != 0)
		return (PATH_NOT_FOUND);
	if (S_ISDIR(statbuf.st_mode))
		return (PATH_IS_DIR);
	fd = open(path, O_RDWR);
	if (fd == -1)
		return (PATH_NO_PERMISSION);
	return (PATH_IS_FILE);
}

t_token	*create_new_chunk(t_token *start)
{
	t_token	*new_chunk;

	new_chunk = tok("", 'g');
	new_chunk->start = start;
	return (new_chunk);
}

void	validate_chunks(t_chunk *head, t_shell_data *shell_data)
{
	int	chunk_num;
	int	status;

	chunk_num = 0;
	while (head)
	{
		status = is_dir(head->tokens[0]);
		if (status == 1)
			gen_err(ERR_ISDIR, head->tokens[0], 126);
		else if (status == 2)
			gen_err(ERR_NOFILDIR, head->tokens[0], 127);
		else if (status == 3)
			gen_err(ERR_NOPERM, head->tokens[0], 126);
		if (status == 1 || status == 2 || status == 3)
			shell_data->start_pos = chunk_num + 1;
		chunk_num++;
		head = head->next;
	}
}

void	process_chunk(t_token *token_chunk, t_token *head,
			t_shell_data *shell_data, int chunk_num)
{
	token_chunk->tokens = tokens2arr(token_chunk, head, shell_data, chunk_num);
	append(token_chunk, &(shell_data->token_chunks));
}

void	chunk_tokens(t_shell_data *shell_data)
{
	t_token	*head;
	t_chunk	*token_chunk;
	int		chunk_num;

	head = shell_data->token_list;
	token_chunk = create_new_chunk(shell_data->token_list);
	chunk_num = 0;
	g_exit_status = 0;
	while (head)
	{
		if (head->type == SH_PIPE)
		{
			process_chunk(token_chunk, head, shell_data, chunk_num);
			token_chunk = create_new_chunk(head->next);
			chunk_num++;
			g_exit_status = 0;
		}
		head = head->next;
	}
	process_chunk(token_chunk, NULL, shell_data, chunk_num);
	head = shell_data->token_chunks;
	//validate_chunks(head, shell_data);
}
