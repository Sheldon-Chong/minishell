#include "minishell.h"

bool	pattern_rec(t_token *pattern_start, t_token_info *token_info)
{
	if (is_strset(pattern_start->word, "|,>,>>,<,<<") && (!pattern_start->next))
		return (syntax_error("newline", token_info), true);
	if (!pattern_start->next)
		return (false);
	if (is_strset(pattern_start->word, "|,>,>>,<,<<"))
	{
		if (is_strset(pattern_start->next->word, "|,>,>>,<,<<"))
			return (syntax_error(pattern_start->next->word, token_info), true);
	}
	return (false);
}

bool	post_validate(t_token_info *token_info)
{
	t_token	*head;

	head = token_info->token_list;
	while (head && !token_info->has_error)
	{
		pattern_rec(head, token_info);
		head = head->next;
	}
	return (true);
}


t_token	*tok(char *word, char type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->word = word;
	token->type = type;
	token->infile = NULL;
	token->outfile = NULL;
	token->start = NULL;
	token->next = NULL;
	token->outfile_mode = 0;
	token->heredoc_buffer = NULL;
	return (token);
}

t_token	*append_tok(t_token *token, t_token **head)
{
	t_token	*nav;

	if (!(token->word))
		return (NULL);
	if (!(*head))
	{
		*head = token;
		token->prev = NULL;
		return (token);
	}
	nav = *head;
	while (nav && nav->next)
		nav = nav->next;
	nav->next = token;
	token->prev = nav;
	return (token);
}
