/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:31 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:17:32 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("getcwd");
		g_exit_status = 1;
		return ;
	}
	printf("%s\n", path);
	free(path);
}
