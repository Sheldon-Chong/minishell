/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:22 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:23 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_env(t_env *env_list)
{
	t_env	*head;

	head = env_list;
	while (head)
	{
		if (head->value != NULL)
			printf("%s=%s\n", head->name, head->value);
		head = head->next;
	}
}
