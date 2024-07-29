#include "minishell.h"


char **dup_doublearray(char **src)
{
    char **output;
    int i = 0;

    while (src[i] != NULL)
        i++;
    output = ft_calloc(i + 1, sizeof(char *));
    if (output == NULL)
        return NULL; // Handle allocation failure
    output[i] = NULL;
    while (--i >= 0)
    {
        output[i] = ft_strdup(src[i]);
        if (output[i] == NULL)
        {
            // Free previously allocated memory on failure
            ft_free_array((void **)output, 0);
            return NULL;
        }
    }
    return output;
}

char **sort_doublearray(char **array)
{
    int i, j, max;
    char **output;
    char *temp;

    output = dup_doublearray(array);
    if (output == NULL)
        return NULL; // Handle allocation failure
    max = 0;
    while (array[max] != NULL)
        max++;
    for (i = 0; i < max; i++)
    {
        for (j = i + 1; j < max; j++)
        {
            if (ft_strcmp(output[i], output[j]) > 0)
            {
                temp = output[i];
                output[i] = output[j];
                output[j] = temp;
            }
        }
    }
    return output;
}


char	*ft_strndup(const char *str, size_t n)
{
	size_t	i;
	char	*output;

	output = malloc(sizeof(char) * n + 1);
	if (output == NULL)
		return (NULL);
	i = -1;
	while (++i < n)
		output[i] = str[i];
	output[i] = '\0';
	return (output);
}

t_env	*str2env(char *str)
{
	t_env	*env;
	char	*env_key;
	char	*env_value;

	if (ft_strchr(str, '=') == NULL)
	{
		return (new_env(ft_strdup(str), NULL));
	}
	env_value = ft_strdup(ft_strchr(str, '=') + 1);
	env_key = ft_substr(str, 0, ft_strlen(str) - ft_strlen(env_value) - 1);

	if (ft_strlen(env_value) == 0)
	{
		free(env_value);
		env_value = ft_strdup("");
	}
	env = new_env(env_key, env_value);

	return (env);
}

static int	export_assign(t_token_info *token_info, char **args)
{
	int		i;
	int		error;
	t_env	*env;
	char	**arr;

	i = 0;
	error = 0;
	while (args[++i] != 0)
	{
		env = str2env(args[i]);
		if (!is_valid_identifier(env->name))
		{
			error = 1;
			printf("-bash: export: '");
			printf("%s", args[i]);
			printf("': not a valid identifier\n");
			continue ;
		}
		append_env(env, &(token_info->env_data->env_list));
	}
	return (error);
}


int	ft_export(char **args, t_token_info *token_info)
{
	char	**arr;

	if (args[1] == NULL)
	{
		arr = env2arr(token_info->env_data->env_list);
		arr = sort_doublearray(arr);
		t_env *head = token_info->env_data->env_list;
		while (head)
		{
			free(head->name);
			free(head->value);
			head = head->next;
		}
		token_info->env_data->env_list = arr2env(arr);
		print_env(&(token_info->env_data->env_list), 'x');
	}
	else
		export_assign(token_info, args);
	return (0);
}
