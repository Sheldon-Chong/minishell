#include "minishell.h"

char	**sort_doublearray(char **array)
{
	int		i;
	int		j;
	int		max;
	char	**output;
	char	*temp;

	output = dup_doublearray(array);
	if (output == NULL)
		return (NULL);
	max = double_arr_len((void **)array);
	i = -1;
	while (++i < max)
	{
		j = i;
		while (++j < max)
		{
			if (ft_strcmp(output[i], output[j]) > 0)
			{
				temp = output[i];
				output[i] = output[j];
				output[j] = temp;
			}
		}
	}
	return (output);
}

void	print_env_export(t_env *env_list)
{
	t_env	*head;

	head = env_list;
	while (head)
	{
		if (head->value == NULL)
			printf("declare -x %s\n", head->name);
		else
			printf("declare -x %s=\"%s\"\n", head->name, head->value);
		head = head->next;
	}
}