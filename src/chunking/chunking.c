/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunking.c                                         :+:      :+:    :+:   */
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

void	process_chunk(t_token *token_chunk, t_token *head,
			t_shell_data *shell_data, int chunk_num)
{
	token_chunk->tokens = group_tokens(token_chunk,
			head, shell_data, chunk_num);
	append(token_chunk, &(shell_data->token_chunks));
}

void	chunk_tokens(t_shell_data *shell_data)
{
	t_token	*head;
	t_chunk	*token_chunk;
	int		chunk_num;

	head = shell_data->token_list;
	token_chunk = new_chunk(shell_data->token_list);
	chunk_num = 0;
	g_exit_status = 0;
	while (head)
	{
		if (head->type == SH_PIPE)
		{
			process_chunk(token_chunk, head, shell_data, chunk_num);
			token_chunk = new_chunk(head->next);
			chunk_num++;
			g_exit_status = 0;
		}
		head = head->next;
	}
	process_chunk(token_chunk, NULL, shell_data, chunk_num);
	head = shell_data->token_chunks;
}

void	clear_heredoc_buffer(t_token *chunk)
{
	if (chunk->infile != NULL)
		chunk->infile = NULL;
	if (chunk->heredoc_buffer != NULL)
	{
		free(chunk->heredoc_buffer);
		chunk->heredoc_buffer = NULL;
	}
}

char	**group_tokens(t_token *chunk, t_token *str_end,
			t_shell_data *shell_data, int num)
{
	t_token	*token;
	char	**cmds;

	cmds = calloc(1, 2 * sizeof(char *));
	token = chunk->start;
	while (token != str_end)
	{
		if (token->type >= SH_WRITE && token->type <= SH_HEREDOC)
		{
			if (handle_redir(token, chunk, shell_data, num) == -1)
				break ;
			if (token->type == SH_HEREDOC)
			{
				clear_heredoc_buffer(chunk);
				chunk->heredoc_buffer
					= here_doc_input(token->next->word, shell_data);
			}
			token = token->next->next;
			continue ;
		}
		cmds = append_to_array(&cmds, token->word);
		token = token->next;
	}
	return (cmds);
}
