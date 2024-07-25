#include "minishell.h"

int	ft_echo(char **args)
{
	int	i;

	i = 1;
	while (args[i] && is_echo_n(args[i]))
		i ++;
	if (i == 1)
	{
		while (args[i])
		{
			printf("%s", args[i]);
			if (args[i + 1])
				printf(" ");
			i ++;
		}
		printf("\n");
	}
	else
		while (args[i])
			printf("%s", args[i++]);
}