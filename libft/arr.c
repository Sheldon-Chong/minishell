#include "libft.h"

int	arr_size(void **array)
{
	int	i;
	
	i = 0;
	while(array[i])
		i ++;
	return (i);
}

int print_arr(void **array)
{
	int	i;
	
	i = -1;
	while(array[++i])
	{
		ft_putstr_fd((char *)(array[i]), 1);
		ft_putchar_fd('\n', 1);
	}
	return 0;
}

int arr_append(void ***array, void *item)
{
	void	**list;
	int		length;
	void	**ret;
	int		i;
	
	list = *array;
	length = arr_size(list);
	print_arr(*array);
	ret = malloc(sizeof(**array) * (length + 2));
	if (!ret)
		return (0);
	i = -1;
	while(++i < length)
		ret[i] = list[i];
	ret[i] = item;
	ret[i + 1] = NULL;
	free(*array);
	*array = ret;
	print_arr(*array);
	return (1);
}