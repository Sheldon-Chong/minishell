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
