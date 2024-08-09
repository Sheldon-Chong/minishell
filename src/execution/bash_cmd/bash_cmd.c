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

int	bash_cmd(t_token_info *token_info, char **args)
{
	int		i;

	if (!ft_strcmp(args[0], "echo"))
		ft_echo(args);
	else if (!ft_strcmp(args[0], "exit"))
	{
		free_tokenlist(token_info);
		ft_exit(0, token_info);
	}
	else if (!ft_strcmp(args[0], "env"))
		print_env(&(token_info->env_data->env_list), 'e');
	else if (!ft_strcmp(args[0], "unset"))
		unset_env(args + 1, &(token_info->env_data->env_list), token_info);
	else if (!ft_strcmp(args[0], "pwd"))
	{
		if (args[1])
			general_error("pwd: too many arguments", NULL, 1);
		else
			ft_pwd();
	}
	else if (!ft_strcmp(args[0], "export"))
		ft_export(args, token_info);
	return (0);
}
