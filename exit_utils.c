#include "minishell.h"


int	ft_str_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	if (i == 1)
	{
		printf("exit\n");
		g_exit_status = 0;
		exit(0);
	}
	else if (i == 2 && ft_strlen(arg[1]) == 0)
	{
		printf("exit\n");
		printf("bash: exit: : numeric argument required\n");
		g_exit_status = 255;
		exit(255);
	}
	else if (i == 2 && ft_str_is_digit(arg[1]))
	{
		i = ft_atoi(arg[1]) % 256;
		printf("exit\n");
		g_exit_status = i;
		exit(i);
	}
	else if (!ft_str_is_digit(arg[1]))
	{
		printf("exit\n");
		printf("bash: exit: ");
		printf("%s", arg[1]);
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
