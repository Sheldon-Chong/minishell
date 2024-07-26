#include "minishell.h"

t_env	*arr2env(char **env)
{
	int		i;
	t_env	*env_list;
	char	*var_name;
	char	*var_value;

	i = -1;
	env_list = NULL;
	while (env[++i])
	{
		var_name = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
		var_value = ft_strdup(ft_strchr(env[i], '=') + 1);
		append_env(new_env(var_name, var_value), &env_list);
	}
	return (env_list);
}

char	**env2arr(t_env *env)
{
	int		i;
	char	**env_arr;
	t_env	*head;

	i = 0;
	head = env;
	while (head)
	{
		i ++;
		head = head->next;
	}
	env_arr = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env)
	{
		char *str = ft_strjoin(ft_strdup(env->name), "=");
		str = ft_strjoin(str, env->value);
		env_arr[i] = str;

		env = env->next;
		i ++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

t_env	*get_env_var(char *var_name, t_env **env)
{
	t_env	*head;

	head = *env;
	while (head)
	{
		if (ft_strcmp(head->name, var_name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}



int	print_env(t_env **env_list, char mode)
{
	t_env	*head;

	head = *env_list;
	while (head)
	{
		if (mode == 'x')
			printf("declare -x %s=\"%s\"\n", head->name, head->value);
		else if (mode == 'e')
			printf("%s=%s\n", head->name, head->value);
		head = head->next;
	}
	return (0);
}

bool is_valid_identifier(char *str)
{
	if (!ft_isalpha(*str) && *str != '_')
		return (false);
	while(*(++str))
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
	}
	return (true);
}

int	export(char **args, t_token_info *token_info)
{
	char	*var_name;
	char	**env_split;

	if (!(args[1]))
		print_env(token_info->global_env, 'x');
	else
	{
		env_split = ft_split(args[1], '=');

		printf("IS VALID?: %d\n", is_valid_identifier(args[1]));
		printf("name: %s\n", env_split[0]);
		printf("value: %s\n", env_split[1]);
		
	}
	return (0);
}
