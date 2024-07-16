#include "minishell.h"

void	ctrl_c_function(int signum)
{
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
	signal(SIGINT, ctrl_c_function);
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
		{
		}
		else if (access(get_path(list->tokens[0],
					token_info->env_arr), F_OK) != 0 \
					|| !strcmp(list->tokens[0], ""))
		{
			printf("bash: %s: command not found\n", list->start->word);
			token_info->cmd_start = list->next;
		}
		list = list->next;
	}
	return (token_info->cmd_start);
}

int	main(int ac, char **av, char **env)
{
	void			(*ctrl_c_handler)(int);
	t_token_info	*token_info;
	char			**env_list;
	HIST_ENTRY		**history;
	char			*user_input;

	ctrl_c_handler = signal(SIGINT, ctrl_c_function);
	signal(SIGQUIT, SIG_IGN);
	history = history_list();
	t_env *global_env = arr2env(env, env_list);

	while (1)
	{
		user_input = readline(SHELL_MSG);
		if (!user_input) //ctrl + D
			break ;
		if (ft_strlen(user_input) > 0)
		{
			add_history(user_input);
			token_info = process_input(user_input, global_env);
			token_info->env_arr = env;
			print_tokens(token_info, 'l');
			if (scan_cmd(token_info) && !token_info->has_error)
				executor(env, token_info); //THIS IS TO BE REPLACED
			free_TokenList(token_info);
		}
	}
	exit(0);
}

