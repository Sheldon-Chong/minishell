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



#include <sys/stat.h>
int is_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir) {
        closedir(dir);
        return 1; // Path is a directory
    } else {
        return 0; // Path is not a directory or an error occurred
    }
}

int is_dir(char *path) {
    struct stat statbuf;
    
	if (!ft_strchr(path, '/'))
		return 0;

	// Check if the path exists
    if (stat(path, &statbuf) != 0) {
        return 2;
    }

    // Check if the path is a directory
    if (S_ISDIR(statbuf.st_mode)) {
        return 1;
    }

	
	int fd = open(path, O_RDWR);
	if (fd == -1)
	{
		return 3;
	}
    // Path exists but is not a directory
    return 0;
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
		if (head->type == SH_PIPE)
		{
			token_chunk->tokens = tokens2arr(token_chunk, head, token_info, chunk_num);
			append_tok(token_chunk, &(token_info->token_chunks));
			token_chunk = tok("", 'g');
			token_chunk->start = head->next;
			chunk_num++;
			g_exit_status = 0;
		}			
		head = head->next;
	}
	token_chunk->tokens = tokens2arr(token_chunk, NULL, token_info, chunk_num);
	append_tok(token_chunk, &(token_info->token_chunks));

	head = token_info->token_chunks;

	chunk_num = 0;
	while (head)
	{
		int status = is_dir(head->tokens[0]);
		if (status == 1)
		{
			general_error("$SUBJECT,: is a directory", head->tokens[0], 126);
			token_info->start_pos = chunk_num + 1;
		}
		else if (status == 2)
		{
			general_error("$SUBJECT,: No such file or directory", head->tokens[0], 127);
			token_info->start_pos = chunk_num + 1;
		}
		else if (status == 3)
		{
			general_error("$SUBJECT,: Permission denied", head->tokens[0], 126);
			token_info->start_pos = chunk_num + 1;
		}
		chunk_num ++;
		head = head->next;
	}
}
