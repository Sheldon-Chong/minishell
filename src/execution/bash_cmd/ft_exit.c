#include "minishell.h"

bool	ft_str_is_digit(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	i = -1;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
	}
	return (true);
}

static void	handle_exit_errors(char **args, int len)
{
	if (len == 2 && ft_strlen(args[1]) == 0)
	{
		printf("exit\n");
		printf("bash: exit: : numeric argument required\n");
		g_exit_status = 255;
		exit(255);
	}
	else if (!ft_str_is_digit(args[1]))
	{
		printf("exit\n");
		printf("bash: exit: ");
		printf("%s", args[1]);
		printf(": numeric argument required\n");
		g_exit_status = 255;
		exit(255);
	}
	else
	{
		printf("exit\n");
		printf("bash: exit: : too many arguments\n");
		g_exit_status = 1;
		return ;
	}
}

void	ft_exit(char **args, t_token_info * token_info)
{
	int	len;

	len = 0;
	while (args[len])
		len++;
	t_env *head = token_info->env_data->env_list;
	while(head)
	{
		t_env *temp = head->next;
		free(head->name);
		free(head->value);
		free(head);
		head = temp;
	}
	// ft_free_array((void **)token_info->env_data->environ_arr, 0);
	free(token_info->env_data->environ_arr);
	// free(token_info->env_data->env_list);
	if (len == 1)
	{
		printf("exit\n");
		g_exit_status = 0;
		free_TokenList(token_info);
		exit(0);
	}
	else if (len == 2 && ft_str_is_digit(args[1]))
	{
		printf("exit\n");
		g_exit_status = ft_atoi(args[1]) % 256;
		free_TokenList(token_info);
		exit(g_exit_status);
	}
	handle_exit_errors(args, len);
}
