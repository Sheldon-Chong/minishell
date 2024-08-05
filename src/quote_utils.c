/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:18:12 by jyap              #+#    #+#             */
/*   Updated: 2024/08/05 09:18:12 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//returns true if it encounters an alternate outermost quote
bool	quote_alternate(char c, char *quote)
{
	if (is_in_charset(c, "\"'") && (!(*quote) || c == *quote))
	{
		*quote = ((!(*quote)) * c);
		return (true);
	}
	return (false);
}

char	toggle_quote_state(char quote, char c)
{
	if (quote && c == quote)
		return (0);
	else if (!quote)
		return (c);
	return (quote);
}

int	count_outermost_quotes(char *str)
{
	int		count;
	char	quote;

	count = 0;
	quote = 0;
	while (*str)
	{
		if ((is_in_charset(*str, "'\""))
			&& (!quote || *str == quote))
		{
			quote = toggle_quote_state(quote, *str);
			count ++;
		}
		str++;
	}
	return (count);
}
