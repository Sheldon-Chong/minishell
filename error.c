#include "minishell.h"

int print_error(t_error error, t_token_info *token_info)
{
    token_info->has_error = true;
    printf("syntax error near unexpected token `%s'\n", error.subject);
    return 1;
}