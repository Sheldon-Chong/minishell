#include "libft.h"

char	*c2str(int c)
{
	char	*buffer;

	buffer = malloc(2);
	buffer[0] = c;
	buffer[1] = 0;

	return (buffer);
}
