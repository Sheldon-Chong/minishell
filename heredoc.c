#include "minishell.h"

// void	heredoc_cmd_queue(t_token *token, t_token_info *token_info, int state)
// {
// 	while (token)
// 	{
// 		if (state == 1)
// 			heredoc_io_list_enqueue(token, token->io_list, token_info);
// 		else
// 			heredoc_io_list_dequeue(token, token->io_list);
// 		token = token->next;
// 	}
// }

// void	heredoc_io_list_enqueue(t_token *token, t_io *io_list, t_token_info *token_info)
// {
// 	while (io_list)
// 	{
// 		if (io_list->e_type == IO_AIN)
// 			heredoc_enqueue(&token->heredocs,
// 				*(char **)io_list->value->content, token_info);
// 		io_list = io_list->next;
// 	}
// }

// void	heredoc_io_list_dequeue(t_token *token, t_io *io_list)
// {
// 	int	fd;

// 	while (io_list)
// 	{
// 		if (io_list->e_type == IO_AIN)
// 		{
// 			fd = heredoc_dequeue(&token->heredocs);
// 			close(fd);
// 		}	
// 		io_list = io_list->next;
// 	}
// }

// void	heredoc_enqueue(t_list **heredoc, char *delimiter, t_token_info *token_info)
// {
// 	char	*buffer;
// 	int		*fd;

// 	fd = ft_calloc(2, sizeof(int));
// 	pipe(fd);
// 	printf("> ");
// 	buffer = get_next_line(STDIN_FILENO);
// 	while (buffer)
// 	{
// 		if (!ft_strncmp(delimiter, buffer, ft_strlen(delimiter))
// 			&& (ft_strlen(delimiter) == ft_strlen(buffer) - 1))
// 		{
// 			free(buffer);
// 			break ;
// 		}
// 		ft_putstr_fd(expand_env(buffer, token_info), fd[1]);
// 		free(buffer);
// 		printf("> ");
// 		buffer = get_next_line(STDIN_FILENO);
// 	}
// 	close(fd[1]);
// 	ft_lstadd_back(heredoc, ft_lstnew(fd));
// }

// int	heredoc_dequeue(t_list **heredoc)
// {
// 	int		res;
// 	t_list	*temp;

// 	temp = *heredoc;
// 	*heredoc = (*heredoc)->next;
// 	res = *(int *)temp->content;
// 	ft_lstdelone(temp, &free);
// 	return (res);
// }