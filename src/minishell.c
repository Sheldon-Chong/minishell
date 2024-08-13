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

t_shell_data	*process_input(char *str, t_env_data *env_data)
{
	t_shell_data	*shell_data;

	shell_data = malloc(sizeof(t_shell_data));
	shell_data->start_pos = 0;
	shell_data->token_list = NULL;
	shell_data->cmd_start = NULL;
	shell_data->quote_list_buffer = NULL;
	shell_data->token_chunks = NULL;
	shell_data->has_error = false;
	shell_data->env_data = env_data;
	if (count_outermost_quotes(str) % 2 != 0)
		err_no_braces(shell_data);
	tokenize(str, shell_data);
	post_validate(shell_data);
	if (shell_data->has_error)
		return (shell_data);
	chunk_tokens(shell_data);
	return (shell_data);
}

void	init_signal(void)
{
	signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int ac, char **av, char **env)
{
	t_shell_data	*shell_data;
	char			*user_input;
	t_env_data		*env_data;

	init_signal();
	env_data = new_env_data(env);
	while (true)
	{
		user_input = readline(SHELL_MSG);
		if (!user_input)
			break ;
		if (ft_strlen(user_input) > 0)
		{
			add_history(user_input);
			shell_data = process_input(user_input, env_data);
			if (!shell_data->token_list)
				continue ;
			if (!shell_data->has_error)
				executor(shell_data);
			//print_tokens(shell_data, 'l');
			free_tokenlist(shell_data);
		}
	}
	clear_history();
	exit(0);
	(void)ac;
	(void)av;
}
