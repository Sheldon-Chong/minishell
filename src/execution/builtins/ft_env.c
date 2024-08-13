#include "minishell.h"

void print_env_env(t_env *env_list) {
	t_env *head = env_list;
	while (head) {
		if (head->value != NULL)
			printf("%s=%s\n", head->name, head->value);
		head = head->next;
	}
}