#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (count == SIZE_MAX || size == SIZE_MAX)
		return (0);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}


int	ft_free_array(void **array, int ret)
{
	size_t	i;

	if (!array)
		return (0);
	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (ret);
}


int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] == s2[i]) && (s1[i] != '\0') && (s2[i] != '\0'))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strdup(const char *s)
{
	char	*str;

	str = (char *)malloc(sizeof(*str) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	return ((char *)ft_memcpy(str, s, (ft_strlen(s) + 1)));
}

char	**dup_doublearray(char **src)
{
	char	**output;
	int		i;

	i = 0;
	while (src[i] != NULL)
		i++;
	output = ft_calloc(i + 1, sizeof(char *));
	if (output == NULL)
		return (NULL);
	output[i] = NULL;
	while (--i >= 0)
	{
		output[i] = ft_strdup(src[i]);
		if (output[i] == NULL)
		{
			ft_free_array((void **)output, 0);
			return (NULL);
		}
	}
	return (output);
}

char	**sort_doublearray(char **array)
{
	int		i;
	int		j;
	int		max;
	char	**output;
	char	*temp;

	output = dup_doublearray(array);
	if (output == NULL)
		return (NULL);
	max = 0;
	while (array[max] != 0)
		max++;
	i = -1;
	while (++i < max)
	{
		j = i;
		while (++j < max)
		{
			if (ft_strcmp(output[i], output[j]) > 0)
			{
				temp = output[i];
				output[i] = output[j];
				output[j] = temp;
			}
		}
	}
	return (output);
}



// Helper function to print a double array (for debugging purposes)
void print_doublearray(char **array) {
    int i = 0;
    while (array[i] != NULL) {
        printf("%s\n", array[i]);
        i++;
    }
}

// Test function for sort_doublearray
void test_sort_doublearray() {
    // Sample input array
    char *input[] = {"banana", "apple", "cherry", "date", NULL};
    
    // Expected output array
    char *expected[] = {"apple", "banana", "cherry", "date", NULL};
    
    // Call the function
    char **output = sort_doublearray(input);
    
    // Verify the output
    int i = 0;
    while (expected[i] != NULL) {
        assert(strcmp(output[i], expected[i]) == 0);
        i++;
    }
    
    // Free the output array
    i = 0;
    while (output[i] != NULL) {
        free(output[i]);
        i++;
    }
    free(output);
    
    printf("All tests passed!\n");
}

int main() {
    test_sort_doublearray();
    return 0;
}