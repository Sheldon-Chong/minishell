# include "minishell.h"

void	reset_signal(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGINT handler");
	if (signal(SIGTERM, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGTERM handler");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGQUIT handler");
}

void ignore_sigint(int signum)
{
	// Do nothing, just catch the signal to prevent termination
}
