/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:19 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:19 by jyap             ###   ########.fr       */
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

int	bash_cmd(t_shell_data *shell_data, char **args)
{
	int		i;

	if (!ft_strcmp(args[0], "echo"))
		ft_echo(args);
	else if (!ft_strcmp(args[0], "exit"))
	{
		free_tokenlist(shell_data);
		ft_exit(0, shell_data);
	}
	else if (!ft_strcmp(args[0], "env"))
		print_env(&(shell_data->env_data->env_list), 'e');
	else if (!ft_strcmp(args[0], "unset"))
		unset_env(args + 1, &(shell_data->env_data->env_list), shell_data);
	else if (!ft_strcmp(args[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(args[0], "export"))
		ft_export(args, shell_data);
	return (0);
}
