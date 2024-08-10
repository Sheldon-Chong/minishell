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
	{
		//free(chunk->infile);
		chunk->infile = NULL;
	}
	if (chunk->heredoc_buffer != NULL)
	{
		free(chunk->heredoc_buffer);
		chunk->heredoc_buffer = NULL;
	}
}

int	handle_redir(t_token *head, t_token *token_chunk, t_shell_data *shell_data, int num)
{
	int	file;

	if (head->type == SH_APPEND || head->type == SH_WRITE)
	{
		
		file = open(head->next->word, O_CREAT | O_RDWR, 0644);
		char *error_message = strerror(errno);
		if (file == -1)
		{
			shell_data->start_pos = num + general_error("$SUBJECT,: Permission denied", head->next->word, 1);
			return -1;
		}
		else
			close(file);
		if (head->type == SH_APPEND)
			token_chunk->outfile_mode = 'a';
		if (token_chunk->heredoc_buffer != NULL)
		{
			free(token_chunk->heredoc_buffer);
			token_chunk->heredoc_buffer = NULL;
		}
		token_chunk->outfile = head->next->word;
	}
	if (head->type == SH_READ)
	{
		token_chunk->infile = head->next->word;
		file = open(token_chunk->infile, O_RDONLY);
		if (file == -1 && g_exit_status == 0)
		{
			shell_data->start_pos = num + general_error("$SUBJECT,: No such file or directory", head->next->word, 1);
			return -1;
		}
	}
	return (0);
}


char	**tokens2arr(t_token *chunk, t_token *str_end, t_shell_data *shell_data, int num)
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
				break;
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

int	get_length_of_list(t_token *head)
{
	int	length;

	length = 0;
	while (head)
	{
		length++;
		head = head->next;
	}
	return (length);
}
