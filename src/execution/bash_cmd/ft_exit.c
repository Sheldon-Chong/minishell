/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:26 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:26 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_str_is_digit(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	if (!ft_isdigit(str[0]) && str[0] != '+' && str[0] != '-')
		return (false);
	i = 0;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
	}
	return (true);
}

void	free_env_data(t_env_data *env_data)
{
	t_env	*head;
	t_env	*tmp;

	head = env_data->env_list;
	while (head)
	{
		tmp = head;
		free(head->name);
		free(head->value);
		head = head->next;
		free(tmp);
	}
	ft_free_array((void **)((env_data)->environ_arr), 0);
	free(env_data);
}

static int	handle_exit_errors(char **args, int len, t_shell_data *shell_data)
{
	if (len == 2 && ft_strlen(args[1]) == 0)
	{
		ft_putstr_fd("minishell: exit: : numeric argument required\n", 2);
		g_exit_status = 255;
		free(shell_data->executor);
		free_env_data(shell_data->env_data);
		free_tokenlist(shell_data);
		exit(g_exit_status);
	}
	else if (!ft_str_is_digit(args[1]))
	{
		gen_err(ERR_EXITNUMARG, args[1], 255);
		g_exit_status = 255;
		free_env_data(shell_data->env_data);
		free(shell_data->executor);
		free_tokenlist(shell_data);
		exit(g_exit_status);
	}
	return (gen_err("exit: too many arguments", NULL, 1));
}

void	ft_exit(char **args, t_shell_data *shell_data)
{
	int	len;

	len = 0;
	while (args[len])
		len++;
	if (len == 1)
	{
		g_exit_status = 0;
		free(shell_data->executor);
		free_env_data(shell_data->env_data);
		free_tokenlist(shell_data);
		exit(0);
	}
	else if (len == 2 && ft_str_is_digit(args[1]))
	{
		g_exit_status = ft_atoi(args[1]) % 256;
		free(shell_data->executor);
		free_env_data(shell_data->env_data);
		free_tokenlist(shell_data);
		exit(g_exit_status);
	}
	handle_exit_errors(args, len, shell_data);
}
