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

void	free_env_list(t_env **envlist)
{
	t_env	*temp;
	if (envlist == NULL || *envlist == NULL)
		return ;
	temp = *envlist;
	while(*envlist != NULL)
	{
		temp = *envlist;
		*envlist = (*envlist)->next;
		free(temp);
	}
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

/*split str to key and value
example A=hello
output[0] = A
output[1] = hello
output[2] = NULL

example A
output[0] = A
output[1] = NULL, which also serves as NULL value
this is different from empty string

example A=""
output[0] = A
output[1] = \0
output[2] = NULL

*/
char	**env_split(char *str)
{
	char	**output;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i] != '\0')
		i++;
	if (str[i] == '\0')
	{
		output = malloc(sizeof(char *) * 2);
		if (output == NULL)
			return (NULL);
		output[0] = ft_strdup(str);
		output[1] = 0;
		return (output);
	}
	output = malloc(sizeof(char *) * 3);
	if (output == NULL)
		return (NULL);
	i++;
	output[0] = ft_strndup(str, i - 1);
	output[1] = ft_strndup(str + i, ft_strlen(str) - i);
	output[2] = 0;
	return (output);
}

/*
replaces one of the env_array
example A=""
export A=abc
will replace the whole A="" string with A=abc
*/
static void	replace_env(char **str, char *key, char *value)
{
	char	*temp;

	free(*str);
	temp = ft_strjoin(key, "=");
	*str = ft_strjoin(temp, value);
	free(temp);
}

/*
if variable not found in env_arr, will create a new copy of the env_arr,
with the last element as the new env, frees the old env_arr and set env_arr
to the new env_arr
example1 export A will just add A as key and no value
example2 export A="" or A= will add A as key and empty string as value
example3 export A=abc will add A as key and abc as value
*/
static void add_new_env(t_token_info *token_info, char *key, char *value, int i)
{
	char	**new_env_arr;
	char	*temp;

	append_env(new_env("OK", "HI"), (token_info->global_env));
	
}

/*
Case 1: if variable is found in env_arr, and the variable has a = sign,
update value if argument has value, else do nothing
Case 2: if variable is found in env_arr, and the variable has no = sign,
update value if argument has value, else do nothing
Case 3: if variable not found in env_arr, add new env
Free old global_env and create new global_evn with new env_arr
*/
static void	update_env(t_token_info *token_info, char *args, char *key, char *value)
{
	int		i;
	char	**temp;

	temp = token_info->env_arr;
	i = -1;
	// while (temp[++i] != NULL)
	// {
	// 	if (ft_strncmp(temp[i], key, ft_strlen(key)) == 0
	// 		&& temp[i][ft_strlen(key)] == '=')
	// 	{
	// 		if (ft_strlen(args) == ft_strlen(key))
	// 			return ;
	// 		replace_env(&temp[i], key, value);
	// 		return ;
	// 	}
	// 	else if (ft_strncmp(temp[i], key, ft_strlen(key) + 1) == 0)
	// 	{
	// 		if (ft_strlen(args) == ft_strlen(key))
	// 			return ;
	// 		replace_env(&temp[i], key, value);
	// 		return ;
	// 	}
	// }
	add_new_env(token_info, key, value, i);
	t_env *head = *(token_info->global_env);
	while(head)
	{
		printf(">> %s\n", head->name);
		head = head->next;
	}
}

/*
split argument into key and value
check key is valid identifier, if not print error message
if valid, update env
free split before going to next argument
return error = 1 if any of the arguments were not valid identifer 
*/
static int	find_and_add(t_token_info *token_info, char **args)
{
	int		i;
	char	**split;
	int		error;

	i = 0;
	error = 0;
	while (args[++i] != 0)
	{
		split = ft_split(args[i], '=');
		if (is_valid_identifier(split[0]))
			append_env(new_env(ft_strdup(split[0]), ft_strdup(split[1])), (token_info->global_env));
		else
		{
			error = 1;
			printf("-bash: export: '");
			printf("%s", args[i]);
			printf("': not a valid identifier\n");
		}
		ft_free_array((void **)split, 0);
	}
	return (error);
}

/*
if no arguments, print sorted env_arr
else go to find_and_add function
*/
int	ft_export(char **args, t_token_info *token_info)
{
	char	**arr;
	
	if (args[1] == NULL)
	{ 
		arr = env2arr(*(token_info->global_env));
		arr = sort_doublearray(arr);
		*(token_info->global_env) = arr2env(arr);
		print_env(token_info->global_env, 'x');
	}
	else
		find_and_add(token_info, args);
	return (0);
}
