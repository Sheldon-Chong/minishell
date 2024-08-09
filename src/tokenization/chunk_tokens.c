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

int	handle_redir(t_token *head, t_token *token_chunk, t_token_info *token_info, int num)
{
	int	file;

	if (head->type == SH_APPEND || head->type == SH_WRITE)
	{
		file = open(head->next->word, O_CREAT | O_RDWR, 0644);
		if (file == -1)
			token_info->has_error = general_error("$subject,: Permission denied\n", head->next->word, 1);
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
		if (file == -1 && g_exit_status == 0)
			token_info->start_pos = num + general_error("$SUBJECT,: No such file or directory", head->next->word, 1);
	}
	return (0);
}


#include <sys/stat.h>
int is_directory(const char *path) {
	struct stat path_stat;
	if (stat(path, &path_stat) != 0) {
		return 0;
	}
	return S_ISDIR(path_stat.st_mode);
}

void	chunk_tokens(t_token_info *token_info)
{
	t_token		*head;
	t_token		*token_chunk;
	int			chunk_num = 0;
	
	head = token_info->token_list;
	token_chunk = tok("", 'g');
	token_chunk->start = token_info->token_list; 
	g_exit_status = 0;
	while (head)
	{
		if (head->type >= SH_WRITE && head->type <= SH_HEREDOC
			&& (!handle_redir(head, token_chunk, token_info, chunk_num)))
			nothing();
		else if (head->type == SH_PIPE)
		{
			token_chunk->tokens = tokens2arr(token_chunk, head, token_info);
			append_tok(token_chunk, &(token_info->token_chunks));
			token_chunk = tok("", 'g');
			token_chunk->start = head->next;
			chunk_num++;
			g_exit_status = 0;
		}			
		head = head->next;
	}
	token_chunk->tokens = tokens2arr(token_chunk, NULL, token_info);
	append_tok(token_chunk, &(token_info->token_chunks));

	head = token_info->token_chunks;

	chunk_num = 0;
	while (head)
	{
		if (head->tokens && is_directory(head->tokens[0]))
		{
			general_error("$SUBJECT,: is a directory", head->tokens[0], 1);
			token_info->start_pos = chunk_num + 1;
		}
		chunk_num ++;
		head = head->next;
	}
}
