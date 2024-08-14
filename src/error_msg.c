/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:01 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:01 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_error(char *error, t_shell_data *shell_data)
{
	shell_data->has_error = true;
	g_exit_status = ERRNO_SYNTAX;
	printf("%s: syntax error near unexpected token `%s'\n", SHELL_MSG, error);
	return (1);
}

int	err_no_braces(t_shell_data *shell_data)
{
	shell_data->has_error = true;
	g_exit_status = ERRNO_SYNTAX;
	printf("%s: syntax error: no closing quotation\n", SHELL_MSG);
	return (1);
}

int	gen_err(char *error, char *subject, int exit_status)
{
	char	**error_splitted;
	int		i;

	error_splitted = ft_split(error, ',');
	i = -1;
	ft_putstr_fd("minishell: ", 2);
	while (error_splitted[++i])
	{
		if (!ft_strcmp(error_splitted[i], "$SUBJECT"))
			ft_putstr_fd(subject, 2);
		else
			ft_putstr_fd(error_splitted[i], 2);
	}
	ft_free_array((void **)error_splitted, 0);
	ft_putstr_fd("\n", 2);
	g_exit_status = exit_status;
	return (exit_status);
}

int	exit_error(char *error_name)
{
	perror(error_name);
	exit(EXIT_FAILURE);
}
