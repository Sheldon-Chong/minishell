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

static void	handle_exit_errors(char **args, int len, t_token_info *token_info)
{
	if (len == 2 && ft_strlen(args[1]) == 0)
	{
		ft_putstr_fd("minishell: exit: : numeric argument required\n", 2);
		g_exit_status = 255;
		free(token_info->executor);
		free_env_data(token_info->env_data);
		free_tokenlist(token_info);
		exit(255);
	}
	else if (!ft_str_is_digit(args[1]))
	{
		ft_putstr_fd("exit\nminishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		g_exit_status = 255;
		free_env_data(token_info->env_data);
		free(token_info->executor);
		free_tokenlist(token_info);
		exit(255);
	}
	ft_putstr_fd("exit\nminishell: exit: too many arguments\n", 2);
	g_exit_status = 1;
	return ;
}

void	ft_exit(char **args, t_token_info *token_info)
{
	int	len;

	len = 0;
	while (args[len])
		len++;
	if (len == 1)
	{
		g_exit_status = 0;
		free(token_info->executor);
		free_env_data(token_info->env_data);
		free_tokenlist(token_info);
		exit(0);
	}
	else if (len == 2 && ft_str_is_digit(args[1]))
	{
		g_exit_status = ft_atoi(args[1]) % 256;
		free(token_info->executor);
		free_env_data(token_info->env_data);
		free_tokenlist(token_info);
		exit(g_exit_status);
	}
	handle_exit_errors(args, len, token_info);
}
