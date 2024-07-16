gcc *.c ./libft/*.c ./tokenization/*.c ./env/*.c -I./includes -I./libft -lncurses -lreadline -o minishell -fsanitize=address -g3 && ./minishell
#gcc *.c ./libft/*.c ./pipex/*.c ./pipex/srcs/*.c -lncurses -lreadline  && valgrind --leak-check=full ./a.out


# -fsanitize=address 
# include "minishell.h" 
# valgrind --leak-check=full 
# -fsanitize=address -g3