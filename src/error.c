# include "minishell.h"

int	syntax_error(char *error, t_token_info *token_info)
{
	token_info->has_error = true;
	g_exit_status = ERR_SYNTAX;
	printf("syntax error near unexpected token `%s'\n", error);
	return (1);
}

int	err_no_braces(char *subject, t_token_info *token_info)
{
	token_info->has_error = true;
	g_exit_status = ERR_SYNTAX;
	printf("bash: syntax error no closing quotation\n");
	return (1);
}
