/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 11:08:18 by shechong          #+#    #+#             */
/*   Updated: 2024/07/16 18:43:44 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <limits.h>
# include <stdbool.h>

/*
if string consist only of characters in charset

Example
-------
str_charset("AB", "A", CSET_WITHIN) = false
str_charset("AB", "BA", CSET_WITHIN) = true
str_charset("AB", "", CSET_WITHIN) = false

str_charset("143", "0123456789", CSET_WITHIN) = true
str_charset("username", "abcdefjhijklmnopqrstuvwxyz", CSET_WITHIN) = true
*/
# define CSET_WITHIN 0

/*
If string contains none of the characters in charset

Example
-------
str_charset("AB", "CDEF", CSET_EXCLUDE) = true
str_charset("AB", "BCDEF", CSET_EXCLUDE) = false
str_charset("", "ABCD", CSET_EXCLUDE) = false
*/
# define CSET_EXCLUDE 1

/*
If string contains one or more characters in charset

Example
-------
str_charset("AB", "CDEF", CSET_CONTAINS) = false
str_charset("AB", "BCDEF", CSET_CONTAINS) = true
str_charset("", "", CSET_CONTAINS) = false

*/
# define CSET_CONTAINS 2

/*
If string contains a character not in charset

Example
-------

str_charset("AB", "CDEF", CSET_CONTAINS) = true
str_charset("AB", "BCDEF", CSET_CONTAINS) = true
str_charset("", "", CSET_CONTAINS) = false

*/
# define CSET_HAS_NEW 3

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

void			ft_bzero(void *s, size_t n);
void			*ft_calloc(size_t count, size_t size);
char			*ft_strdup(const char *s1);

int				ft_isalnum(int c);
int				ft_isalpha(int c);
int				ft_isascii(int c);
int				ft_isdigit(int c);
int				ft_isprint(int c);

int				ft_ibounds(long num);
void			ft_free_stack(t_list **head);
int				ft_free_array(void **array, int ret);

int				ft_contains_char(char *str, char *chars);
int				ft_is_charset(char *str, char *charset);

int				ft_atoi(const char *str);
long			ft_atol(const char *str);
char			*ft_itoa(int n);

void			*ft_memccpy(void *dst, const void *src, int c, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);
void			*ft_memset(void *b, int c, size_t len);

void			ft_putchar_fd(char c, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
void			ft_putstr_fd(char *s, int fd);

char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void			ft_striteri(char *s, void (*f)(unsigned int, char*));

char			*ft_rstrjoin(char **s1, char *s2);
char			*ft_strjoin(char const *s1, char const *s2);

size_t			ft_strlcat(char *dst, const char *src, size_t size);
size_t			ft_strlcpy(char *dst, const char *src, size_t size);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_rsubstr(char **s1, unsigned int start, size_t len);
char			**ft_split(char const *s, char c);
char			*ft_strtrim(char const *s1, char const *set);

size_t			ft_strlen(const char *s);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
char			*ft_strnstr(const char *haystack, const char *needle,
					size_t len);

char			*ft_strrchr(const char *s, int c);
char			*ft_strchr(const char *s, int c);

int				ft_tolower(int c);
int				ft_toupper(int c);

t_list			*ft_lstnew(void *content);
int				ft_lstsize(t_list *lst);
t_list			*ft_lstlast(t_list *lst);
void			ft_lstadd_front(t_list **alst, t_list *new);
void			ft_lstadd_back(t_list **alst, t_list *new);
void			ft_lstdelone(t_list *lst, void (*del)(void*));
void			ft_lstclear(t_list **lst, void (*del)(void*));
void			ft_lstiter(t_list *lst, void (*f)(void*));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void *));

char			*get_next_line(int fd);

int				ft_instr(char c, char *str);

void			*f(void *ptr, void *input);
void			*fa(void **ptr, void *input);

bool			is_in_charset(int c, char *charset);
bool			str_charset(char *str, char *charset, int mode);
bool			is_in_strset(char *str, char *strset);

int				nothing(void);

char			*c2str(int c);

int				arr_size(void **array);
int				print_arr(void **array);
int				arr_append(void ***array, void *item);

bool			str_in_arr(char *str, char	*str_array);
int				ft_strcmp(const char *s1, const char *s2);

char			*ft_fstrdup(char **s);
char			*ft_fstrjoinf(char **s1, char **s2);

int				double_arr_len(void **array);
char			**sort_doublearray(char **array);

#endif
