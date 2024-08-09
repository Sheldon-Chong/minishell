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

int	syntax_error(char *error, t_token_info *token_info)
{
	token_info->has_error = true;
	g_exit_status = ERR_SYNTAX;
	printf("syntax error near unexpected token `%s'\n", error);
	return (1);
}

int	err_no_braces(char *subject, t_token_info *token_info)
{
	token_info->has_error = true;
	g_exit_status = ERR_SYNTAX;
	printf("minishell: syntax error no closing quotation\n");
	return (1);
}

int general_error(char *error, char *subject, int exit_status)
{
	char **error_splitted = ft_split(error, ',');
	int i = -1;

	ft_putstr_fd("minishell: ", 2);
	while(error_splitted[++i])
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