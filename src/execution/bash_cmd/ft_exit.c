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
	i = -1;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
	}
	return (true);
}

void free_env_data (t_env_data *env_data)
{
	t_env *head = (env_data)->env_list;
	while(head)
	{
		t_env *tmp = head;
		free(head->name);
		free(head->value);
		head = head->next;
		free(tmp);
	}
	ft_free_array((void**)((env_data)->environ_arr), 0);
	free(env_data);
}

static void	handle_exit_errors(char **args, int len, t_token_info *token_info)
{
	if (len == 2 && ft_strlen(args[1]) == 0)
	{
		printf("exit\n");
		printf("minishell: exit: : numeric argument required\n");
		g_exit_status = 255;
		free(token_info->executor);
		free_env_data(token_info->env_data);
		free_tokenlist(token_info);
		exit(255);
	}
	else if (!ft_str_is_digit(args[1]))
	{
		printf("exit\n");
		printf("minishell: exit: ");
		printf("%s", args[1]);
		printf(": numeric argument required\n");
		g_exit_status = 255;
		free_env_data(token_info->env_data);
		free(token_info->executor);
		free_tokenlist(token_info);
		exit(255);
	}
	else
	{
		printf("exit\n");
		printf("minishell: exit: too many arguments\n");
		g_exit_status = 1;
		return ;
	}
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
