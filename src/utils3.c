/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:46:40 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 18:18:28 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup(t_shell_data *shell_data)
{
	return ;
}

int	exit_error(char *error_name)
{
	perror(error_name);
	exit(EXIT_FAILURE);
}