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

# include <sys/stat.h>
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
# include <errno.h>
# include <fcntl.h>
#include <termios.h>
# include "shared_types.h"

extern int	g_exit_status;

# define SHELL_MSG "minishell$ "
# define SPACE_CHAR " \f\v\t\r\n"
# define INVALID_ENV_CHAR "[]<>!@$%%^-+~/&() "
# define BASH_CMDS "env,cd,unset,export,exit,echo,pwd"
# define BRACE -125
# define INVALID_CHARS "{};\\"
# define SHELL_OPERATORS "|><"

# define ERR_NOFILDIR "$SUBJECT,: No such file or directory"
# define ERR_NOPERM "$SUBJECT,: Permission denied"
# define ERR_CMDNOTFOUND "$SUBJECT, : command not found"
# define ERR_EXITNUMARG "exit: ,$SUBJECT,: numeric argument required"
# define ERR_EXPORTINVALID "export: `,$SUBJECT,' : not a valid identifier"
# define ERR_ISDIR "$SUBJECT,: is a directory"

# define NOT_WITHIN_QUOTE '\0'

# define ERRNO_INVALID_EXIT_ARG 128
# define ERRNO_COMMAND_NOT_FOUND 127
# define ERRNO_NO_PERMISSIONS 126
# define ERRNO_SYNTAX 258
# define ERRNO_CTRL_C 130

# define SH_STR 0
# define SH_PIPE 1
# define SH_WRITE 2
# define SH_APPEND 3
# define SH_READ 4 
# define SH_HEREDOC 5

# define PATH_IS_DIR 1
# define PATH_IS_FILE 0
# define PATH_NO_PERMISSION 3
# define PATH_NOT_FOUND 2

# define P_FILE 0
# define P_DIR 1
# define P_NOT_EXIST 2

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

typedef struct s_shell_data
{
	t_token		*token_list;
	t_token		*quote_list_buffer;
	t_chunk		*token_chunks;
	int			size;
	char		*str;
	t_token		*cmd_start;
	bool		has_error;
	t_env		**global_env;
	t_env_data	*env_data;
	t_executor	*executor;
	int			start_pos;
}	t_shell_data;

typedef struct s_error
{
	char	error_type;
	char	*subject;
}	t_error;

void			ignore_sigint(int signum);
t_env			*get_env_var(char *var_name, t_env **env);

// utils
int				get_length_of_list(t_token *head);

int				ft_cd(t_chunk *chunk);

void			free_env_data(t_env_data *env_data);
int				free_env_list(t_env *env);
void			print_env_export(t_env *env_list);
void			print_env_env(t_env *env_list);
char			**dup_doublearray(char **src);

// debugging
int				print_tokens(t_shell_data *token_list, char format);

// free utils
int				free_shell_data(t_shell_data *token_list);

// tokenization
t_token			*tokenize(char *string, t_shell_data *token_list);
char			**group_tokens(t_token *chunk, t_token *str_end,
					t_shell_data *token_info, int num);

t_token			*append(t_token *token, t_token **head);
t_token			*tok(char *word, char type);
int				is_token_valid(char *str, t_shell_data *token_info);
t_shell_data	*process_input(char *str, t_env_data *env_data);
bool			post_validate(t_shell_data *token_info);

// environment variables
t_env			*arr2env(char **env);
t_env			*get_env_var(char *var_name, t_env **env);
int				print_env(t_env **env_list, char mode);
char			**env2arr(t_env *env);
char			*expand_env(char *string, t_shell_data *token_info);
t_env			*new_env(char *name, char *value);
int				unset_env(char **var_names, t_env **envList,
					t_shell_data *token_info);
void			ft_pwd(void);

void			append_env(t_env *env, t_env **envList);
int				export(char **args, t_shell_data *token_info);
bool			is_strset(char *str, char *strset);
bool			is_valid_identifier(char *str);
t_env_data		*new_env_data(char **env);

// quotes
char			toggle_quote_state(char quote, char c);
char			*split_into_quotes(char *str, t_token *tokens,
					t_shell_data *token_info, bool expand_env);
int				count_outermost_quotes(char *str);
bool			quote_alternate(char c, char *quote);
int				gen_err(char *error, char *subject, int exit_status);

//chunking
void			chunk_tokens(t_shell_data *token_list);

// to be removed
void			executor(t_shell_data *token_info);
t_chunk			*new_chunk(t_token *start);

int				handle_redir(t_token *head, t_token *token_chunk,
					t_shell_data *shell_data, int num);
// error printing
int				syntax_error(char *error, t_shell_data *token_info);
int				err_no_braces(t_shell_data *token_info);

int				builtins(t_shell_data *token_info, char **cmd);

void			exec_cmd(char **cmd, t_shell_data *token_info,
					int cmd_in_fd, int cmd_out);
char			*get_path(char *cmd, t_env **env);
void			parse_cmd_list_for_io(t_shell_data *token_info);
char			**append_to_array(char ***array, char *value);
char			*here_doc_input(char *delimiter, t_shell_data *shell_data);
bool			is_pure_env(char *str);

void			ft_exit(char **arg, t_shell_data *token_info);
int				ft_export(char **args, t_shell_data *token_info);
int				ft_echo(char **args);
bool			is_echo_n(char *str);

t_executor		*executor_init(void);
int				exit_error(char *error_name);
void			reset_signal(void);
void			dup_fd_for_child(int cmd_in_fd, int cmd_out);
void			close_fds(int cmd_in_fd, int cmd_out);
void			ctrl_c_function(int signum);
int				find_env_end(char *env_start);
int				get_shell_opp_type(char *str);
int				handle_read(t_token *head, t_token *token_chunk,
					t_shell_data *shell_data, int num);
bool			is_char_transition_quote(char quote_status, char current_char);

t_chunk			*get_chunk_start(t_token *start, int pos);
void			set_outf(t_token *chunk_list, t_executor *exe);
void			set_inf(t_executor *executor, t_chunk *chunk_list,
					t_shell_data *shell_data);
void			set_child_redirections(t_shell_data *shell_data);
int				check_path_type(const char *path);
void			free_env_node(t_env *node);
void			child(t_chunk *chunk_list, t_shell_data *shell_data);
void			parent(t_chunk *chunk_list, t_shell_data *shell_data);
int				run_cmd(t_token *chunk, t_shell_data *shell_data,
					int cmd_in_fd, int cmd_out);

#endif
