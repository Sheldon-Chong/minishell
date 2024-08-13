#include "minishell.h"

int ft_cd(t_chunk *chunk)
{
    if (chunk->tokens[2])
    {
        g_exit_status = 0;
        return (0);
    }
    if (chdir(chunk->tokens[1]) == -1)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        perror(chunk->tokens[1]);
        g_exit_status = 1;
    }
}
