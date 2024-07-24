#include "libft.h"

/*
Checks if a character can be found in a given charset
*/
bool	is_in_charset(int c, char *charset)
{
	int		i;
	bool	ret;

	ret = false;
	i = -1;
	while (charset[++i])
		if (charset[i] == c)
			return (true);
	return (ret);
}

/*
Check if a string contains the characters specified from charset 

str: string that will be scanned
charset: set that [str] will be scanned against
mode = CSET_CONTAINS, CSET_EXCLUDE, CSET_HAS_NEW, CSET_WITHIN

return: returns true/false based on if the string meets the conditions of [mode]
*/
bool	str_charset(char *str, char *charset, int mode)
{
	int		i;
	bool	ret;

	i = 0;
	ret = true;
	if (mode == CSET_CONTAINS || mode == CSET_HAS_NEW)
		ret = false;

	while (str[i])
	{
		if (!is_in_charset(str[i], charset) && mode == CSET_WITHIN)
			ret = false;
		if (is_in_charset(str[i], charset) && mode == CSET_EXCLUDE)
			ret = false;
		if (is_in_charset(str[i], charset) && mode == CSET_CONTAINS)
			return (true);
		if (!is_in_charset(str[i], charset) && mode == CSET_HAS_NEW)
			return (true);
		i ++;
	}
	return (ret);
}


bool	is_in_strset(char *str, char *strset)
{
	char	**set;
	int		i;

	set = ft_split(strset, ',');
	if (!str || !set || !(set[0]))
		return (false);
	i = -1;
	while (set[++i])
	{
		if (!ft_strncmp(str, set[i], ft_strlen(set[i])))
			return (ft_free_array((void **)set, true));
	}
	ft_free_array((void **)set, 0);
	return (false);
}

bool	is_strset(char *str, char *strset)
{
	char	**set;
	int		i;

	set = ft_split(strset, ',');
	if (!str || !set || !(set[0]))
		return (false);
	i = -1;
	while (set[++i])
	{
		if (!ft_strcmp(str, set[i]))
			return (ft_free_array((void **)set, true));
	}
	ft_free_array((void **)set, 0);
	return (false);
}

