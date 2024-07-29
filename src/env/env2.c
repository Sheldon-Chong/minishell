#include "minishell.h"

t_env	*new_env(char *name, char *value)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->next = NULL;
	env->name = name;
	env->value = value;
	return (env);
}

t_env	*append_env(t_env *env, t_env **envList)
{
	t_env	*head;

	if (!(*envList))
	{
		*envList = env;
		return (env);
	}
	head = *envList;
	while (head)
	{
		if (!ft_strcmp(head->name, env->name))
		{
			head->value = env->value;
			return (head);
		}
		head = head->next;
	}
	head = *envList;
	while (head->next)
		head = head->next;
	head->next = env;
	return (env);
}

int	unset_env(char **var_names, t_env **envList, t_token_info *token_info)
{
	t_env	*head;
	t_env	*prev;
	t_env	*temp;
	int		i;

	i = 0;
	while (var_names[i])
	{
		head = *envList;
		prev = NULL;
		while (head)
		{
			if (!ft_strcmp(var_names[i], head->name))
			{
				if (prev)
					prev->next = head->next;
				else
					*envList = head->next;
				temp = head;
				head = head->next;
				free(temp->name);
				free(temp->value);
				free(temp);
				break ;
			}
			prev = head;
			head = head->next;
		}
		i++;
	}
	ft_free_array((void **)token_info->env_arr, 0);
	token_info->env_arr = env2arr(*envList);
	return (0);
}
