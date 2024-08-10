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

#define PATH_IS_DIR 1
#define PATH_IS_FILE 0
#define PATH_NO_PERMISSION 3
#define PATH_NOT_FOUND 2

int is_dir(char *path)
{
    struct stat statbuf;
    
	if (!ft_strchr(path, '/'))
		return (0);

	// Check if the path exists
    if (stat(path, &statbuf) != 0)
        return (PATH_NOT_FOUND);

    // Check if the path is a directory
    if (S_ISDIR(statbuf.st_mode))
        return (PATH_IS_DIR);
	
	int fd = open(path, O_RDWR);
	if (fd == -1)
		return (PATH_NO_PERMISSION);
    // Path exists but is not a directory
    return (0);
}

void process_chunk(t_token *token_chunk, t_token *head, t_token_info *token_info, int chunk_num)
{
    token_chunk->tokens = tokens2arr(token_chunk, head, token_info, chunk_num);
    append_tok(token_chunk, &(token_info->token_chunks));
}

t_token *create_new_chunk(t_token *start)
{
    t_token *new_chunk = tok("", 'g');
    new_chunk->start = start;
    return new_chunk;
}

void validate_chunks(t_token *head, t_token_info *token_info)
{
    int chunk_num = 0;

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
        chunk_num++;
        head = head->next;
    }
}

void chunk_tokens(t_token_info *token_info)
{
    t_token *head = token_info->token_list;
    t_token *token_chunk = create_new_chunk(token_info->token_list);
    int chunk_num = 0;

    g_exit_status = 0;

    while (head)
    {
        if (head->type == SH_PIPE)
        {
            process_chunk(token_chunk, head, token_info, chunk_num);
            token_chunk = create_new_chunk(head->next);
            chunk_num++;
            g_exit_status = 0;
        }
        head = head->next;
    }

    process_chunk(token_chunk, NULL, token_info, chunk_num);
    head = token_info->token_chunks;

    validate_chunks(head, token_info);
}

