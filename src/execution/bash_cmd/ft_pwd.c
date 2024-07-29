#include "minishell.h"

void	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		perror("getcwd");
		g_exit_status = 1;
		return ;
	}
	printf("%s\n", path);
	free(path);
}
