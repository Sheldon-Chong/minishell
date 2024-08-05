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
	return (0);
}
