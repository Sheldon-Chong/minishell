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

void	ft_exit(char **args)
{
	int	len;

	len = 0;
	while (args[len])
		len++;
	if (len == 1)
	{
		printf("exit\n");
		g_exit_status = 0;
		exit(0);
	}
	else if (len == 2 && ft_str_is_digit(args[1]))
	{
		printf("exit\n");
		g_exit_status = ft_atoi(args[1]) % 256;
		exit(g_exit_status);
	}
	handle_exit_errors(args, len);
}
