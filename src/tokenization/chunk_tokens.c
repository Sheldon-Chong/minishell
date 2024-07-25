#include "minishell.h"

int	handle_redir(t_token *head, t_token *token_chunk, t_token_info *token_info)
{
	int	file;

	if (str_in_arr(head->word, ">,>>"))
	{
		file = open(head->next->word, O_CREAT | O_RDWR, 0644);
		if (file == -1)
		{
			printf("bash: %s: Permission denied\n", head->next->word);
			token_info->has_error = true;
		}
		else
			close(file);
		if (!ft_strcmp(head->word, ">>"))
			token_chunk->outfile_mode = 'a';
		if (token_chunk->heredoc_buffer != NULL)
		{
			free(token_chunk->heredoc_buffer); 
			token_chunk->heredoc_buffer = NULL;
		}
		token_chunk->outfile = head->next->word;
	}
	if (!ft_strcmp(head->word, "<"))
		token_chunk->infile = head->next->word;
	head = head->next->next;
	return (0);
}

void	chunk_tokens(t_token_info *token_info)
{
	t_token		*head;
	t_token		*token_chunk;

	head = (token_info->token_list);
	token_chunk = tok("", 'g');
	token_chunk->start = token_info->token_list;
	while (head)
	{
		if (str_in_arr(head->word, ">,>>,<<,<")
			&& (!handle_redir(head, token_chunk, token_info)))
		{
			head = head->next;
			continue ;
		}
		else if (head->type == T_PIPE)
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
