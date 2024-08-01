/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_in_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 11:47:36 by shechong          #+#    #+#             */
/*   Updated: 2023/04/30 14:16:18 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

bool	str_in_arr(char *str, char	*str_array)
{
	char	**array;
	int		i;

	arrat = ft_split(str_array, ',');
	i = -1;
	if (!str || !array || !(array[0]))
		return (false);
	while (array[++i])
	{
		if (!ft_strcmp(str, array[i]))
			return (ft_free_array((void **)array, true));
	}
	return (ft_free_array((void **)array, false));
}
