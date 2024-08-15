/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:03 by jyap              #+#    #+#             */
/*   Updated: 2024/08/15 15:43:54 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_signal(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGINT handler");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGQUIT handler");
}

// Do nothing, just catch the signal to prevent termination
void	ignore_sigint(int signum)
{
	(void)signum;
	return ;
}

void	ctrl_c_function(int signo)
{
	if (signo != SIGINT)
		return ;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = ERRNO_CTRL_C;
}

void	ctrl_c_heredoc(int signum)
{
	(void)signum;
	printf("\n");
	close(STDIN_FILENO);
	g_exit_status = ERRNO_CTRL_C;
}
