/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:22 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:23 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_echo_n(char *str)
{
	int	i;

	i = 0;
	if (str[0] != '-')
		return (false);
	while (str[++i])
	{
		if (str[i] != 'n')
			return (false);
	}
	return (true);
}

int	ft_echo(char **args)
{
	int	arg;
	int	newline;

	arg = 1;
	newline = 1;
	while (args[arg] && is_echo_n(args[arg]))
	{
		newline = 0;
		arg++;
	}
	while (args[arg])
	{
		printf("%s", args[arg]);
		if (args[arg + 1])
			printf(" ");
		arg++;
	}
	if (newline)
		printf("\n");
	return (0);
}
