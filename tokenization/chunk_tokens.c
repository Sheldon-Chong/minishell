#include "minishell.h"

int	handle_redir(t_token *head, t_token *token_chunk)
{
	int	file;

	if (head->next && head->next->type != T_PIPE)
	{
		if (str_in_arr(head->word, ">,>>"))
		{
			file = open(head->next->word, O_CREAT | O_RDWR, 0644);
			if (file == -1)
				printf("bash: %s: Permission denied\n", head->next->word);
			else
				close(file);
			head->outfile_mode = 'a' * (!ft_strcmp(head->word, ">>"));
			token_chunk->outfile = head->next->word;
		}
		if (!ft_strcmp(head->word, "<"))
			token_chunk->infile = head->next->word;
		if (!ft_strcmp(head->word, "<<"))
			token_chunk->heredoc = head->next->word;
		head = head->next->next;
		return (1);
	}
	else
		printf("bash: syntax error near unexpected token_chunk `newline'\n");
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
			&& (!handle_redir(head, token_chunk)))
		{
			head = head->next;
			continue ;
		}
		else if (head->type == T_PIPE)
		{
			token_chunk->cmds = tokens2arr(token_chunk, head);
			append_tok(token_chunk, &(token_info->token_chunks));
			token_chunk = tok("", 'g');
			token_chunk->start = head->next;
		}
		head = head->next;
	}
	token_chunk->cmds = tokens2arr(token_chunk, NULL);
	append_tok(token_chunk, &(token_info->token_chunks));
}
