#include "minishell.h"

bool pattern_rec(t_token *pattern_start, t_token_info *token_info)
{
	if (is_in_strset(pattern_start->word, "|,>,>>,<,<<") && (!pattern_start->next))
		return (print_error((t_error){'s', "newline"}, token_info), true); 
	if (!pattern_start->next)
		return (false);
	if (is_in_strset(pattern_start->word, "|,>,>>,<,<<"))
	{
		if (is_in_strset(pattern_start->next->word, "|,>,>>,<,<<"))
			return (print_error((t_error){'s', pattern_start->next->word}, token_info), true);
	}
	return (false);
}

bool post_validate(t_token_info *token_info)
{
	t_token *head = token_info->token_list;

	while(head && !token_info->has_error)
	{
		pattern_rec(head, token_info);
		head = head->next;
	}
	return 1;
}

t_token_info	*process_input(char *str, t_env *global_env)
{
	t_token_info	*token_info;

	token_info = malloc(sizeof(t_token_info));
	token_info->token_list = NULL;
	token_info->cmd_start = NULL;
	token_info->token_list = NULL;
	token_info->quote_list_buffer = NULL;
	token_info->token_chunks = NULL;
	token_info->has_error = false;
	token_info->env_arr = NULL;
	token_info->global_env = global_env;

	tokenize(str, token_info);
	post_validate(token_info);

	if (token_info->has_error)
		return (token_info);
	chunk_tokens(token_info);
	return (token_info);
}

t_token	*tok(char *word, char type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	token->word = word;
	token->type = type;
	token->infile = NULL;
	token->outfile = NULL;
	token->outfile_mode = 0;
	token->start = NULL;
	token->heredoc = NULL;
	token->next = NULL;
	return (token);
}

int	print_tokens(t_token_info *token_info, char format)
{
	t_token	*head2;

	head2 = token_info->token_list;
	printf("\n===============\n");
	int i2 = 0;
	printf("%s>> TOKENIZATION: \n%sLocate tokens, process them and append them to token_list\n%stoken_list\n", C_BBLUE, C_GREEN, C_RESET);
	while (head2)
	{
		printf("	TOKEN %d [%s]: %c\n", i2, head2->word, head2->type);
		head2 = head2->next;
		i2 ++;
	}
	printf("%s\n>> TOKEN CHUNK: \n%sGroup tokens into arrays, based on '|' characters\n%scmd_array\n", C_BBLUE, C_GREEN, C_RESET);
	head2 = token_info->token_chunks;
	i2 = 0;
	while (head2)
	{
		int i = -1;
		printf("	TOKEN %d: [", i2);
		while(head2->tokens[++i])
			printf("\"%s\", ", head2->tokens[i]);
		printf("] (%c), infile: %s, outfile: %s\n", head2->type, head2->infile, head2->outfile);
		head2 = head2->next;
		i2 ++;
	}
	printf("%s\n>> EXCEVE:\n%s", C_BBLUE, C_RESET);
	if (!(token_info->cmd_start == NULL))
		printf("	start at: [%s]\n", token_info->cmd_start->tokens[0]);
	else
		printf("	start at: None\n");
	printf("\n===============\n");
	return (0);
}

t_token	*append_tok(t_token *token, t_token **head)
{
	t_token	*nav;

	if (!(token->word))
		return (NULL);
	if (!(*head))
	{
		*head = token;
		return (token);
	}
	nav = *head;
	while (nav && nav->next)
		nav = nav->next;
	nav->next = token;
	return (token);
}
