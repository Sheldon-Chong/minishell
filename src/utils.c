/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:14 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:14 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	newline(int var)
{
	rl_on_new_line();
	return (1);
}

int	is_token_valid(char *str, t_shell_data *shell_data)
{
	char	quote;
	int		i;

	i = -1;
	quote = 0;
	while (str[++i])
	{
		if (is_in_charset(str[i], "'\""))
			quote = toggle_quote_state(quote, str[i]);
		if (!quote && is_in_charset(str[i], INVALID_CHARS))
		{
			shell_data->has_error = true;
			printf("minishell: syntax error near unexpected token `%c'\n",
				str[i]);
			return (1);
		}
	}
	if (quote)
	{
		shell_data->has_error = true;
		printf("minishell: syntax error near unexpected token `%c'\n", str[i]);
		rl_on_new_line();
		return (1);
	}
	return (0);
}

void	clear_heredoc_buffer(t_token *chunk)
{
	if (chunk->infile != NULL)
		chunk->infile = NULL;
	if (chunk->heredoc_buffer != NULL)
	{
		free(chunk->heredoc_buffer);
		chunk->heredoc_buffer = NULL;
	}
}

int	handle_redir(t_token *head, t_token *token_chunk,
		t_shell_data *shell_data, int num)
{
	int		file;
	int		e;

	e = 0;
	if (head->type == SH_APPEND || head->type == SH_WRITE)
	{
		file = open(head->next->word, O_CREAT | O_RDWR, 0644);
		if (file == -1)
		{
			e = gen_err(ERR_NOPERM, head->next->word, 1);
			shell_data->start_pos = num + e;
			return (-1);
		}
		close(file);
		if (head->type == SH_APPEND)
			token_chunk->outfile_mode = 'a';
		token_chunk->outfile = head->next->word;
	}
	if (head->type == SH_READ)
	{
		e = handle_read(head, token_chunk, shell_data, num);
		if (e == -1)
			return (-1);
	}
	return (0);
}

char	**tokens2arr(t_token *chunk, t_token *str_end,
			t_shell_data *shell_data, int num)
{
	t_token	*token;
	char	**cmds;

	cmds = calloc(1, 2 * sizeof(char *));
	token = chunk->start;
	while (token != str_end)
	{
		if (token->type >= SH_WRITE && token->type <= SH_HEREDOC)
		{
			if (handle_redir(token, chunk, shell_data, num) == -1)
				break ;
			if (token->type == SH_HEREDOC)
			{
				clear_heredoc_buffer(chunk);
				chunk->heredoc_buffer = here_doc_input(token->next->word);
			}
			token = token->next->next;
			continue ;
		}
		cmds = append_to_array(&cmds, token->word);
		token = token->next;
	}
	return (cmds);
}
