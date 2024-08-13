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

void	skip_dollar_char(char *env_start)
{
	while (*env_start == '$')
		env_start++;
}

int	get_length_of_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

void	print_export(t_env *head)
{
	if (head->value == NULL)
		printf("declare -x %s\n", head->name);
	else
		printf("declare -x %s=\"%s\"\n", head->name, head->value);
}
