#include "minishell.h"

int syntax_error(t_error error, t_token_info *token_info)
{
    token_info->has_error = true;
    g_exit_status = ERR_SYNTAX;
    printf("syntax error near unexpected token `%s'\n", error.subject);
    return 1;
}
