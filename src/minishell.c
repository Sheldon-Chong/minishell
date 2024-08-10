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

volatile sig_atomic_t	g_exit_status;


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
	char		*path;

	list = token_info->token_chunks;
	token_info->cmd_start = token_info->token_chunks;
	while (list && list->tokens[0])
	{
		path = get_path(list->tokens[0],
				&(token_info->env_data->env_list));
		if (str_in_arr(list->tokens[0], BASH_CMDS))
			nothing();
		list = list->next;
	}
	return (token_info->cmd_start);
}

t_token_info	*process_input(char *str, t_env_data *env_data)
{
	t_token_info	*token_info;

	token_info = malloc(sizeof(t_token_info));
	token_info->start_pos = 0;
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
	return (token_info);
}



int	main(int ac, char **av, char **env)
{
	t_token_info	*token_info;
	char			*user_input;
	t_env_data		*env_data;

	env_data = new_env_data(env);
	while (1)
	{
		signal(SIGINT, ctrl_c_function);
		signal(SIGQUIT, SIG_IGN);
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
				executor(env_data->environ_arr, token_info);
			//print_tokens(token_info, 'l');
			free_tokenlist(token_info);
		}
	}
	exit(0);
}