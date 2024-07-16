#include "minishell.h"

t_env	*arr2env(char **env, char **env_arr)
{
	int		i;
	t_env	*env_list;
	char	*var_name;

	i = -1;
	env_list = NULL;
	while (env[++i])
	{
		var_name = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
		append_env(new_env(var_name, ft_strchr(env[i], '=') + 1),
			&env_list, env_arr);
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
		env_arr[i++] = ft_strdup(env->name);
		env = env->next;
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

int	export(t_token_info *token_info)
{
	char	**args;
	char	*var_name;

	args = token_info->cmd_start->cmds;
	if (!(token_info->token_list->next))
		print_env(&token_info->global_env, 'x');
	else
	{
		if (ft_strchr(token_info->token_list->next->word, '='))
		{
			var_name = ft_substr(token_info->token_list->next->word, 0,
					(size_t)(ft_strchr(token_info->token_list->next->word, '=')
						- token_info->token_list->next->word));
			if (str_charset(var_name, INVALID_ENV_CHAR, CSET_CONTAINS))
				newline(printf("'%s' not valid identifier\n", var_name));
			else
				append_env(new_env(var_name, \
					ft_strdup(ft_strchr(\
						token_info->token_list->next->word, '=') + 1)), \
						&token_info->global_env, token_info->env_arr);
		}
		else if (str_charset(args[1], INVALID_ENV_CHAR, CSET_CONTAINS))
			newline(printf("'%s' not valid identifier\n", args[1]));
	}
	return (0);
}
