/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*                                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 11:47:36 by shechong          #+#    #+#             */
/*   Updated: 2023/04/30 14:16:18 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*remove_char(char *str, char to_remove)
{
	char	*ret;
	int		i;
	int		count;
	int		j;

	count = 0;
	i = -1;
	while (str[++i])
		count += (str[i] == to_remove);
	ret = malloc(ft_strlen(str) - count + 1);
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (str[j])
	{
		if (str[j] != to_remove)
			ret[i++] = str[j];
		j++;
	}
	ret[i] = '\0';
	return (ret);
}
