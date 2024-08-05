/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:09 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:09 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

void	ctrl_c_function(int signum)
{
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = CTRL_C;
}

// an initial check to see if commands do exist
t_token	*scan_cmd(t_token_info *token_info)
{
	t_token		*list;

	list = token_info->token_chunks;
	token_info->cmd_start = token_info->token_chunks;
	while (list && list->tokens[0])
	{
		if (str_in_arr(list->tokens[0], BASH_CMDS))
			nothing();
		else if (access(get_path(list->tokens[0],
					&(token_info->env_data->env_list)), F_OK) != 0 \
					|| !strcmp(list->tokens[0], ""))
		{
			printf("bash: %s: command not found\n", list->start->word);
			token_info->cmd_start = list->next;
			g_exit_status = ERR_COMMAND_NOT_FOUND;
		}
		list = list->next;
	}
	return (token_info->cmd_start);
}

t_token_info	*process_input(char *str, t_env_data *env_data)
{
	t_token_info	*token_info;
	t_token			*head;

	token_info = malloc(sizeof(t_token_info));
	token_info->token_list = NULL;
	token_info->cmd_start = NULL;
	token_info->quote_list_buffer = NULL;
	token_info->token_chunks = NULL;
	token_info->has_error = false;
	token_info->env_data = env_data;
	if (count_outermost_quotes(str) % 2 != 0)
		err_no_braces("", token_info);
	tokenize(str, token_info);
	post_validate(token_info);
	if (token_info->has_error)
		return (token_info);
	chunk_tokens(token_info);
	head = token_info->token_chunks;
	while (head)
		head = head->next;
	return (token_info);
}

int	main(int ac, char **av, char **env)
{
	t_token_info	*token_info;
	char			*user_input;
	t_env_data		*env_data;

	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, SIG_IGN);
	env_data = new_env_data(env);
	while (1)
	{
		user_input = readline(SHELL_MSG);
		if (!user_input)
			break ;
		if (ft_strlen(user_input) > 0)
		{
			add_history(user_input);
			token_info = process_input(user_input, env_data);
			if (!token_info->token_list)
				continue ;
			if (scan_cmd(token_info) && !token_info->has_error)
				executor(env, token_info);
			free_tokenlist(token_info);
		}
	}
	exit(0);
}
