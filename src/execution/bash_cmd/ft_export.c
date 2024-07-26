#include "minishell.h"

char	**dup_doublearray(char **src)
{
	char	**output;
	int		i;

	i = 0;
	while (src[i] != 0)
		i++;
	output = ft_calloc(i + 1, sizeof(char *));
	output[i] = 0;
	while (--i >= 0)
		output[i] = ft_strdup(src[i]);
	return (output);
}

char	**sort_doublearray(char **array)
{
	int		i;
	int		j;
	int		max;
	char	**output;
	char	*temp;

	output = dup_doublearray(array);
	max = 0;
	while (array[max] != 0)
		max++;
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


t_env *str2env(char *str)
{
	t_env	*env;
	char	*env_key;
	char	*env_value;

	if (ft_strchr(str, '=') == NULL)
	{
		env = new_env(str, NULL);
		return (env);
	}
	env_value = ft_strdup(ft_strchr(str, '=') + 1);
	env_key = ft_substr(str, 0, ft_strlen(str) - ft_strlen(env_value) - 1);

	if(ft_strlen(env_value) == 0)
	{
		free(env_value);
		env_value=ft_strdup("");
	}

	printf("%s, %s\n", env_key, env_value);
	env = new_env(env_key, env_value);

	return (env);
}

/*
split argument into key and value
check key is valid identifier, if not print error message
if valid, update env
free split before going to next argument
return error = 1 if any of the arguments were not valid identifer 
*/
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
		if (is_valid_identifier(env->name))
		{
			append_env(env, token_info->global_env);
		}
		else
		{
			error = 1;
			printf("-bash: export: '");
			printf("%s", args[i]);
			printf("': not a valid identifier\n");
		}
	}
	return (error);
}



int	ft_export(char **args, t_token_info *token_info)
{
	char **arr;
	if (args[1] == NULL)
	{
		printf("EXPORT\n");
		arr = env2arr(*(token_info->global_env));
		arr = sort_doublearray(arr);
		*(token_info->global_env) = arr2env(arr);
		print_env(token_info->global_env, 'x');
	}
	else
		export_assign(token_info, args);
	return (0);
}
