/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c2str.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 11:47:36 by shechong          #+#    #+#             */
/*   Updated: 2023/04/30 14:16:18 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*c2str(int c)
{
	char	*buffer;

	buffer = malloc(2);
	buffer[0] = c;
	buffer[1] = 0;
	return (buffer);
}
