/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shechong <shechong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 11:47:36 by shechong          #+#    #+#             */
/*   Updated: 2023/04/30 14:16:18 by shechong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <sys/stat.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "colors.h"
# include "libft.h"
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <dirent.h>
# include <stdbool.h>
# include <stdarg.h>
#include <errno.h>
# include <fcntl.h>

# define SHELL_MSG "minishell$ "
# define SPACE_CHAR " \f\v\t\r\n"
# define INVALID_ENV_CHAR "[]<>!@$%%^-+~/&() "
# define BASH_CMDS "env,cd,unset,export,exit,echo,pwd"
# define BRACE -125
# define INVALID_CHARS "{};\\"
# define SHELL_OPERATORS "|><"

# define NOT_WITHIN_QUOTE '\0'

# define ERR_INVALID_EXIT_ARG 128
# define ERR_COMMAND_NOT_FOUND 127
# define ERR_NO_PERMISSIONS 126
# define ERR_SYNTAX 258
# define CTRL_C 130



# define SH_STR 0
# define SH_PIPE 1
# define SH_WRITE 2
# define SH_APPEND 3
# define SH_READ 4 
# define SH_HEREDOC 5


typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*word;			// for token only
	char			type;			// for both
	char			*infile;		//token chunk only
	char			*outfile;		//token chunk only
	char			outfile_mode;	//token chunk only
	char			**tokens;			//token chunk only
	struct s_token	*start;			//token chunk only
	char			*heredoc_buffer;
	struct s_token	*next;			//for BOTH tokens and token chunks
	bool			hasError;
	struct s_token	*prev;
	t_list			*heredocs;
}	t_token;

typedef struct s_env_data
{
	t_env	*env_list;
	char	**environ_arr;
}	t_env_data;

typedef struct s_executor
{
	int		pipefd[2];
	int		cmd_in;
	int		cmd_out;
	int		status;
}	t_executor;

typedef struct s_token_info
{
	t_token		*token_list;
	t_token		*quote_list_buffer;
	t_token		*token_chunks;
	int			size;
	char		*str;
	t_token		*cmd_start;
	char		**env_arr;
	bool		has_error;
	t_env		**global_env;
	t_env_data	*env_data;
	t_executor	*executor;
	int			start_pos;
}	t_token_info;

typedef struct s_error
{
	char	error_type;
	char	*subject;
}	t_error;

// utils
int				get_length_of_list(t_token *head);
int				newline(int var);

// debugging
int				print_tokens(t_token_info *token_list, char format);

// free utils
int				free_tokenlist(t_token_info *token_list);

// tokenization
t_token			*scan_cmd(t_token_info *token_list);
t_token			*tokenize(char *string, t_token_info *token_list);
char	**tokens2arr(t_token *chunk, t_token *str_end, t_token_info *token_info, int num);

t_token			*append_tok(t_token *token, t_token **head);
t_token			*tok(char *word, char type);
int				is_token_valid(char *str, t_token_info *token_info);
t_token_info	*process_input(char *str, t_env_data *env_data);
bool			post_validate(t_token_info *token_info);

// environment variables
t_env			*arr2env(char **env);
t_env			*get_env_var(char *var_name, t_env **env);
int				print_env(t_env **env_list, char mode);
char			**env2arr(t_env *env);
char			*expand_env(char *string, t_token_info *token_info);
t_env			*new_env(char *name, char *value);
int				unset_env(char **var_names, t_env **envList,
					t_token_info *token_info);
void			ft_pwd(void);

t_env			*append_env(t_env *env, t_env **envList);
int				export(char **args, t_token_info *token_info);
bool			is_strset(char *str, char *strset);
bool			is_valid_identifier(char *str);
t_env_data		*new_env_data(char **env);

// quotes
char			toggle_quote_state(char quote, char c);
char			*split_into_quotes(char *str, t_token *tokens,
					t_token_info *token_info, bool expand_env);
int				count_outermost_quotes(char *str);
bool			quote_alternate(char c, char *quote);
int general_error(char *error, char *subject, int exit_status);

//chunking
void			chunk_tokens(t_token_info *token_list);

// to be removed
void			executor(char **env, t_token_info *token_info);

// error printing
int				syntax_error(char *error, t_token_info *token_info);
int				err_no_braces(char *subject, t_token_info *token_info);

int				bash_cmd(t_token_info *token_info, char **cmd);

void			exec_cmd(char **cmd, t_token_info *token_info,
					int cmd_in_fd, int cmd_out);
char			*get_path(char *cmd, t_env **env);
void			parse_cmd_list_for_io(t_token_info *token_info);
char			**append_to_array(char ***array, char *value);
char			*here_doc_input(char *delimiter);

void			ft_exit(char **arg, t_token_info *token_info);
int				ft_export(char **args, t_token_info *token_info);
extern volatile sig_atomic_t g_exit_status;
int				ft_echo(char **args);
bool			is_echo_n(char *str);

char			*ft_strndup(const char *str, size_t n);
int				add_substr_to_toklist(const char *str, int start,
					int len, t_token **tokens);
t_executor		*executor_init(void);
void			set_infile(t_token *chunk_list, t_executor *exe);
void			set_outfile(t_token *chunk_list, t_executor *exe, int *pipefd);
int				exit_error(char *error_name);
void			reset_signal(void);
void			dup_fd_for_child(int cmd_in_fd, int cmd_out);
void			close_fds(int cmd_in_fd, int cmd_out);
void	ctrl_c_function(int signum);
int	find_env_end(char *env_start);

#endif
