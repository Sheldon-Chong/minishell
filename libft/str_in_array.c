#include "libft.h"

bool	str_in_arr(char *str, char	*str_array)
{
	char **array = ft_split(str_array, ',');
	int i = -1;
	if(!str || !array || !(array[0]))
		return (false);
	while(array[++i])
	{
		if(!ft_strcmp(str, array[i]))
			return (ft_free_array((void **)array, true));
	}
	return (ft_free_array((void **)array, false));
}