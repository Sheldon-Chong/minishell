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
		ft_free_array((void **)head->tokens, 0);
		free(head);
		head = temp;
	}
	t_env *env_head = *(token_info->global_env);
	// t_env *env_temp;
	// while(env_head)
	// {
	// 	env_temp = env_head->next;
	// 	free(env_head->value);
	// 	free(env_head->name);
	// 	free(env_head);
	// 	env_head = env_temp;
	// }
	free(token_info);
	return (0);
}
