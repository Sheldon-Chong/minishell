#include "minishell.h"

int	free_TokenList(t_token_info *token_info)
{
	int		i;
	t_token	*head;
	t_token	*temp;

	head = token_info->token_list;
	while (head)
	{
		temp = head->next;
		free(head->word);
		free(head);
		head = temp;
	}
	head = token_info->token_chunks;
	while (head)
	{
		temp = head->next;
		free(head->tokens);
		free(head);
		head = temp;
	}
	free(token_info);
	return (0);
}
