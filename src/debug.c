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
/*
#include "minishell.h"

int	print_tokens(t_shell_data *shell_data, char format)
{
	t_token	*head2;
	int		i2;
	int		i;

	i2 = 0;
	i = -1;
	head2 = shell_data->token_list;
	printf("\n===============\n");
	printf("%s>> TOKENIZATION: \n%segment tokens append them
		to token_list\n%stoken_list\n",
		C_BBLUE, C_GREEN, C_RESET);
	while (head2)
	{
		printf("	TOKEN %d [%s]: ", i2, head2->word);
		if (head2->type == SH_APPEND)
			printf("APPEND");
		else if (head2->type == SH_WRITE)
			printf("WRITE TO");
		else if (head2->type == SH_HEREDOC)
			printf("HEREDOC");
		else if (head2->type == SH_READ)
			printf("READ FROM");
		else
			printf("(none) %d", head2->type);
		printf("\n");
		head2 = head2->next;
		i2++;
	}
	printf("%s\n>> TOKEN CHUNK: \n%sGroup tokens into arrays, 
		based on '|' characters\n%scmd_array\n", C_BBLUE, C_GREEN, C_RESET);
	head2 = shell_data->token_chunks;
	printf("starting position index: %d\n", shell_data->start_pos);
	i2 = 0;
	while (head2)
	{
		i = -1;
		printf("	TOKEN %d: [", i2);
		while (head2->tokens[++i])
			printf("\"%s\", ", head2->tokens[i]);
		printf("] (%c), infile: %s, outfile: %s mode [%c] 
			heredoc_buffer %s\n", head2->type, head2->infile, 
			head2->outfile, head2->outfile_mode, head2->heredoc_buffer);
		head2 = head2->next;
		i2 ++;
	}
	printf("%s\n>> IOLIST%s\n", C_BBLUE, C_RESET);
	printf("%s\n>> EXCEVE:\n%s", C_BBLUE, C_RESET);
	if (!(shell_data->cmd_start == NULL))
		printf("	start at: [%s]\n", shell_data->cmd_start->tokens[0]);
	else
		printf("	start at: None\n");
	printf("\n===============\n");
	return (0);
}
*/