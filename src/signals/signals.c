/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:03 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:03 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_signal(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGINT handler");
	if (signal(SIGTERM, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGTERM handler");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		perror("Failed to reset SIGQUIT handler");
}

// Do nothing, just catch the signal to prevent termination
void	ignore_sigint(int signum)
{
	return ;
}

void	ctrl_c_function(int signum)
{
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
	g_exit_status = ERRNO_CTRL_C;
}