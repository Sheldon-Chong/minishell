#include "libft.h"

int get_array_len(char **array)
{
	int i = 0;
	while(array[i])
		i++;
	return (i);
}
#include <stdio.h>

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
		new_array[0] = ft_strdup(value);
		new_array[1] = NULL;
		return (new_array);
	}
	new_array = malloc(sizeof(char *) * (array_len + 2));
	i = -1;
	while(current_array[++i])
	{
		new_array[i] = ft_strdup(current_array[i]);
	}
	new_array[i] = ft_strdup(value);
	new_array[i + 1] = NULL;
	return new_array;
}