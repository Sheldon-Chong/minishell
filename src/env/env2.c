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
	print_env(envList, 'x');
	return (env);
}

int	unset_env(char *var_name, t_env **envList, t_token_info *token_info)
{
	t_env	*head;
	t_env	*next;
	t_env	*prev;

	head = *envList;
	while (head)
	{
		if (head->next && !ft_strcmp(var_name, head->next->name)) 
		{
			prev = head;
			if (head->next->next) 
			{
				next = head->next->next;
				prev->next = next;
			}
			else
				prev->next = NULL;
		}
		head = head->next;
	}
	ft_free_array((void **)token_info->env_arr, 0);
	token_info->env_arr = env2arr(*envList);
	return (0);
}