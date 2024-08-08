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

int	handle_redir(t_token *head, t_token *token_chunk, t_token_info *token_info)
{
	int	file;

	if (head->type == SH_APPEND || head->type == SH_WRITE)
	{
		file = open(head->next->word, O_CREAT | O_RDWR, 0644);
		if (file == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(head->next->word, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			token_info->has_error = true;
			g_exit_status = 1;
		}
		else
			close(file);
		if (head->type == SH_APPEND)
			token_chunk->outfile_mode = 'a';
		if (token_chunk->heredoc_buffer != NULL)
		{
			free(token_chunk->heredoc_buffer);
			token_chunk->heredoc_buffer = NULL;
		}
		token_chunk->outfile = head->next->word;
	}
	if (head->type == SH_READ)
	{
		token_chunk->infile = head->next->word;
		file = open(token_chunk->infile, O_RDONLY);
		if (file == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(head->next->word, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			token_info->has_error = true;
			g_exit_status = 1;
		}
	}
	head = head->next->next;
	return (0);
}

void	chunk_tokens(t_token_info *token_info)
{
	t_token		*head;
	t_token		*token_chunk;

	head = token_info->token_list;
	token_chunk = tok("", 'g');
	token_chunk->start = token_info->token_list;
	while (head)
	{
		if (head->type >= SH_WRITE && head->type <= SH_HEREDOC
			&& (!handle_redir(head, token_chunk, token_info)))
		{
			head = head->next;
			continue ;
		}
		else if (head->type == SH_PIPE)
		{
			token_chunk->tokens = tokens2arr(token_chunk, head, token_info);
			append_tok(token_chunk, &(token_info->token_chunks));
			token_chunk = tok("", 'g');
			token_chunk->start = head->next;
		}
		head = head->next;
	}
	token_chunk->tokens = tokens2arr(token_chunk, NULL, token_info);
	append_tok(token_chunk, &(token_info->token_chunks));
}
