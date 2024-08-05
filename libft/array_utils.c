/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 11:47:36 by shechong          #+#    #+#             */
/*   Updated: 2023/04/30 14:16:18 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stdio.h>

int	get_array_len(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

char	**append_to_array(char ***array, char *value)
{
	char	**new_array;
	char	**current_array;
	int		array_len;
	int		i;

	current_array = *array;
	array_len = get_array_len(current_array);
	if (current_array[0] == NULL)
	{
		new_array = malloc(sizeof(char *) * 2);
		free(*array);
		new_array[0] = ft_strdup(value);
		new_array[1] = NULL;
		return (new_array);
	}
	new_array = malloc(sizeof(char *) * (array_len + 2));
	i = -1;
	while (current_array[++i])
		new_array[i] = ft_strdup(current_array[i]);
	new_array[i] = ft_strdup(value);
	new_array[i + 1] = NULL;
	ft_free_array((void **)current_array, 0);
	free(**array);
	free(*array);
	return (new_array);
}
