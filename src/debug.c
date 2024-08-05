/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:57 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:57 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

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
		while (head2->tokens[++i])
			printf("\"%s\", ", head2->tokens[i]);
		printf("] (%c), infile: %s, outfile: %s mode [%c] heredoc_buffer %s\n", head2->type, head2->infile, head2->outfile, head2->outfile_mode, head2->heredoc_buffer);
		head2 = head2->next;
		i2 ++;
	}
	printf("%s\n>> IOLIST%s\n", C_BBLUE, C_RESET);

	printf("%s\n>> EXCEVE:\n%s", C_BBLUE, C_RESET);
	if (!(token_info->cmd_start == NULL))
		printf("	start at: [%s]\n", token_info->cmd_start->tokens[0]);
	else
		printf("	start at: None\n");
	printf("\n===============\n");
	return (0);
}


