#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <fcntl.h>

# define SHELL_MSG "minishell$ "
# define SPACE_CHAR " \f\v\t\r\n"
# define INVALID_ENV_CHAR "[]<>!@$%%^-+~/&() "
# define BASH_CMDS "env,cd,unset,export,exit,echo,pwd"
# define BRACE -125
# define INVALID_CHARS "{};\\"
# define SHELL_OPERATORS "|><"

# define T_PIPE 'p'
# define T_CMD 'c'
# define T_REDIR 'r'


# define NOT_WITHIN_QUOTE '\0'



# define ERR_INVALID_EXIT_ARG 128
# define ERR_COMMAND_NOT_FOUND 127
# define ERR_NO_PERMISSIONS 126
# define ERR_SYNTAX 258
# define CTRL_C 130

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_io
{
	enum
	{
		IO_AIN,
		IO_AOUT,
		IO_IN,
		IO_OUT
	}	e_type; //type of redirection, AIN<<, AOUT>>, IN<, OUT>
	//t_list		*value;
	//struct s_io	*next;
	char		*infile_name;
	char		*outfile_name;
	char		*heredoc_buffer;
}	t_io;

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
	t_io			*io_list;		//for token chunk only
	struct s_token	*next;			//for BOTH tokens and token chunks
	struct s_token	*prev;
	t_list			*heredocs;
}	t_token;

bool is_echo_n(char *str);


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
	t_env		*global_env;
}	t_token_info;




typedef struct s_error
{
	char	error_type;
	char	*subject;
}	t_error;

typedef struct s_executor
{
	int pipefd[2];
	int cmd_in;
	int cmd_out;
	int status;
}	t_executor;


// utils
int				get_length_of_list(t_token *head);
int				newline(int var);

// debugging
int				print_tokens(t_token_info *token_list, char format);

// free utils
int				free_TokenList(t_token_info *token_list);

// tokenization
t_token			*scan_cmd(t_token_info *token_list);
t_token_info	*process_input(char *str, t_env *env_arr);
t_token			*tokenize(char *string, t_token_info *token_list);
char			**tokens2arr(t_token *token_chunk, t_token *str_end, t_token_info *token_info);
t_token			*append_tok(t_token *token, t_token **head);
t_token			*tok(char *word, char type);
int				is_token_valid(char *str, t_token_info *token_info);

// environment variables
t_env			*arr2env(char **env, char **env_arr);
t_env			*get_env_var(char *var_name, t_env **env);
int				print_env(t_env **env_list, char mode);
char			**env2arr(t_env *env);
char			*expand_env(char *string, t_token_info *token_info);
t_env			*new_env(char *name, char *value);
int				unset_env(char *var_name, t_env **envList, 
					t_token_info *token_list);
t_env			*append_env(t_env *env, t_env **envList, char **env_arr);
int				export(char **args, t_token_info *token_info);
bool			is_strset(char *str, char *strset);

// quotes
char			toggle_quote_state(char quote, char c);
char			*split_into_quotes(char *str, t_token *tokens, t_token_info *token_info, bool expand_env);
int				count_outermost_quotes(char *str);
bool			quote_alternate(char c, char *quote);

//chunking
void			chunk_tokens(t_token_info *token_list);

// to be removed
void			executor(char **env, t_token_info *token_info);

// error printing
int	syntax_error(char *error, t_token_info *token_info);
int	err_no_braces(char *subject, t_token_info *token_info);

int	bash_cmd(char **env, t_token_info *token_info, char **cmd);

void	exec_cmd(char **cmd, char **env, t_token_info *token_info, int cmd_in_fd, int cmd_out);
char	*get_path(char *cmd, char **env);
void	parse_cmd_list_for_io(t_token_info *token_info);
char	**append_to_array(char ***array, char *value);
char *here_doc_input(char *delimiter);

void	ft_exit(char **arg, t_token_info *token_info);

extern int	g_exit_status;
int	ft_echo(char **args);

#endif
