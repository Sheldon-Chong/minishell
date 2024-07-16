#include "minishell.h"


int	bash_cmd(char **env, t_token_info *token_info)
{
	char	**args;
	int		i;

	args = token_info->cmd_start->cmds;
	if ((!ft_strcmp(args[0], "echo")) && args[1] && !ft_strcmp(args[1], "-n"))
	{
		i = 1;
		while (args[++i])
			printf("%s", args[i]);
	}
	if (!ft_strcmp(args[0], "exit"))
	{
		free_TokenList(token_info);
		exit(0);
	}
	if (!ft_strcmp(args[0], "cd"))
		chdir(args[1]);
	if (!ft_strcmp(args[0], "env"))
		print_env(&token_info->global_env, 'e');
	if (!ft_strcmp(args[0], "unset") && args[1])
		unset_env(args[1], &token_info->global_env, token_info);
	if (!ft_strcmp(args[0], "export"))
		export(token_info);
	return (0);
}

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
	while (list && list->cmds[0])
	{
		if (str_in_arr(list->cmds[0], BASH_CMDS))
		{
		}
		else if (access(get_path(list->cmds[0],
					token_info->env_arr), F_OK) != 0 \
					|| !strcmp(list->cmds[0], ""))
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
			if (scan_cmd(token_info) && !token_info->has_error)
			{
				if (str_in_arr(token_info->cmd_start->cmds[0], BASH_CMDS))
					bash_cmd(env, token_info);
				else if (get_length_of_list(token_info->token_chunks) == 1)
					run_cmds(env, token_info); //THIS IS TO BE REPLACED
			}
			print_tokens(token_info, 'l');
			free_TokenList(token_info);
		}
	}
	exit(0);
}

