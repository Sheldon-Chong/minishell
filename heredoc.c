#include "minishell.h"

typedef struct {
    bool is_cancelled;
} CancelState;

void cancel(int signum)
{
    // Use a static pointer to access the cancellation state
    static CancelState *cancel_state = NULL;
    if (cancel_state == NULL) {
        cancel_state->is_cancelled = true;
    }
}

char *here_doc_input(char *delimiter)
{
	void			(*ctrl_c_handler)(int);
	bool			is_cancelled;
	
	//is_cancelled = false;
	//ctrl_c_handler = signal(SIGINT, cancel);
	write(STDOUT_FILENO, "> ", 2);
	char *buffer = get_next_line(STDIN_FILENO);
	char *str = ft_strdup("");
	CancelState cancel_state = (CancelState){false};

	while (!cancel_state.is_cancelled && buffer)
	{
		if (!ft_strncmp(delimiter, buffer, ft_strlen(delimiter))
			&& (ft_strlen(delimiter) == ft_strlen(buffer) - 1))
			break ;
		ft_rstrjoin(&str, buffer);
		free(buffer);
		write(STDOUT_FILENO, "> ", 2);
		buffer = get_next_line(STDIN_FILENO);
	}
	return (free(buffer), str);
}