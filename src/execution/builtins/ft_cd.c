/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:17:22 by jyap              #+#    #+#             */
/*   Updated: 2024/08/14 16:31:47 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_chunk *chunk)
{
	if (chdir(chunk->tokens[1]) == -1)
	{
		gen_err("cd: ,$SUBJECT,: No such file or directory",
			chunk->tokens[1], 1);
		return (1);
	}
	return (0);
}
