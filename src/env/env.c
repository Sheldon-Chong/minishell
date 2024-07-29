#include "minishell.h"


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
		{
			if (head->value == NULL)
				printf("declare -x %s\n", head->name);
			else
				printf("declare -x %s=\"%s\"\n", head->name, head->value);
		}
		else if (mode == 'e')
			printf("%s=%s\n", head->name, head->value);
		head = head->next;
	}
	return (0);
}

bool	is_valid_identifier(char *str)
{
	if (!ft_isalpha(*str) && *str != '_')
		return (false);
	while (*(++str))
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (false);
	}
	return (true);
}
